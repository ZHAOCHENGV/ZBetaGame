// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ZBPlayerCharacter.h"

#include "AbilitySystem/ZBAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/ZBPlayerState.h"


AZBPlayerCharacter::AZBPlayerCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 1500.0;
	SpringArmComponent->bDoCollisionTest = false;
	
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(SpringArmComponent,USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	SpringArmComponent->bUsePawnControlRotation = true;	
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = true;
	SpringArmComponent->bInheritYaw = true;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

/**
 * @brief PossessedBy 重载实现（服务器/本地都会调用）
 * @param NewController 新的 Controller（可能为 AIController 或 PlayerController）
 * @return void
 *
 * 功能说明：
 *   - 当 Pawn 被 Controller 占有时触发。服务器端通常在此处完成 PlayerState 与 Pawn 的关联，并初始化 AbilitySystemComponent 的 Owner/Avatar。
 *   - 客户端侧 PlayerState 是通过复制到来的，客户端常在 OnRep_PlayerState 做初始化；因此 PossessedBy 以服务器端初始化为主。
 *
 * 详细流程：
 *   1. 调用 Super::PossessedBy 保持基类行为；
 *   2. 缓存 PlayerState 指针（安全检查）；
 *   3. 若为服务器端（HasAuthority）则调用 InitAbilityActorInfo 完成 ASC 初始化，并在服务器授予初始 Ability（若尚未授予）；
 *   4. 可选在本地玩家上执行输入/摄像机初始化（示例中标注但不实现）；
 *
 * 注意事项：
 *   - Owner 与 Avatar 的选取需项目统一（示例常用 Owner = PlayerState, Avatar = Pawn）；
 *   - 授予 Ability 的操作应仅在服务器端执行，且要幂等（使用 bAbilitiesGiven 标记避免重复授予）。
 */
void AZBPlayerCharacter::PossessedBy(AController* NewController)
{
	// 调用父类的 PossessedBy，保留引擎默认行为（例如设置 Controller 指针等）
	Super::PossessedBy(NewController);
	
	//服务器初始化
	InitAbilityActorInfo();
}

/**
 * @brief OnRep_PlayerState 重载实现（客户端在 PlayerState 复制完成后会调用）
 * @return void
 *
 * 功能说明：
 *   - 当 PlayerState 在客户端复制完成后触发。客户端通常在这里初始化 AbilitySystemComponent 的 ActorInfo（客户端展示绑定）、
 *     绑定 UI 更新回调，并恢复与服务器一致的显示状态。
 *
 * 详细流程：
 *   1. 调用 Super::OnRep_PlayerState 保持基类行为；
 *   2. 缓存复制过来的 PlayerState 指针；
 *   3. 调用 InitAbilityActorInfo 完成客户端侧 ASC 初始化（幂等）；
 *   4. 绑定客户端需要的属性/标签变化回调以更新 UI（通过 ViewModel 为佳，示例以注释说明）。
 *
 * 注意事项：
 *   - 不要在客户端执行授予服务器端专属的 Ability/被动 GE（这些应由服务器授予并通过复制同步）；
 *   - 此函数可能会多次调用，InitAbilityActorInfo 必须是幂等的以避免重复绑定。
 */
void AZBPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//客户端初始化
	InitAbilityActorInfo();
}


void AZBPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


/**
   * @brief 初始化或重置 AbilitySystemComponent 的 ActorInfo（Owner/Avatar）的封装函数
   * @return void
   *
   * 功能说明：
   *   - 统一在服务器与客户端的适当时机调用，建立 AbilitySystemComponent 的 Owner/Avatar 指向，
   *     并根据角色的状态完成必要的绑定或授予（授予仅在服务器进行）。
   *
   * 详细流程：
   *   1. 检查 AbilitySystemComponent 是否有效；
   *   2. 选择 OwnerActor（优先 PlayerState，否则使用 this）与 AvatarActor（通常为 this）；
   *   3. 调用 AbilitySystemComponent->InitAbilityActorInfo(OwnerActor, AvatarActor);
   *   4. 在客户端绑定属性/标签监听；在服务器端根据 bAbilitiesGiven 决定是否授予初始 Ability。
   *
   * 注意事项：
   *   - 确保函数实现为幂等（重复调用不会重复授予或多次绑定）；
   *   - 初始化前不要访问 ASC 的属性或绑定（防止空指针）；
   *   - Owner 与 Avatar 的策略在项目中需统一并在注释中说明。
   */
void AZBPlayerCharacter::InitAbilityActorInfo()
{
	//检测PlayerState是否有效
	AZBPlayerState* ZBPlayerState = GetPlayerState<AZBPlayerState>();
	check(ZBPlayerState);

	//设置Owner/Avatar
	ZBPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ZBPlayerState,this);
	//本角色缓存 ASC / AttributeSet，统一从 PS 获取（保持与 GAS 绑定一致）
	AbilitySystemComponent = ZBPlayerState->GetAbilitySystemComponent();
	AttributeSet = ZBPlayerState->GetAttributeSet();
	InitializeDefaultAttributes();
	
}


void AZBPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AZBPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

