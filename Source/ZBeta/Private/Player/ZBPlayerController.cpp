// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZBPlayerController.h"

#include <rapidjson/internal/ieee754.h>

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/ZBAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/ZBEnhancedInputComponent.h"

AZBPlayerController::AZBPlayerController()
{
	bReplicates = true;
}

void AZBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 不崩溃，只警告
	ensureMsgf(DefaultInputMappingContext, TEXT("DefaultInputMappingContext 没有设置！"));

	// 获取增强输入系统 LocalPlayerSubsystem（PlayerController 才能正确 GetLocalPlayer）
	UEnhancedInputLocalPlayerSubsystem * Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInput Subsystem 获取成功!!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInput Subsystem 获取失败!!!"));
	}
	
	// 设置鼠标/输入模式
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void AZBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UZBEnhancedInputComponent* ZBInputComponent = CastChecked<UZBEnhancedInputComponent>(InputComponent);
	if (ZBInputComponent && InputConfig)
	{
		// 原生输入绑定
		ZBInputComponent->BindAction(InputConfig->MoveAction,ETriggerEvent::Triggered,this,&AZBPlayerController::Input_Move);
		ZBInputComponent->BindAction(InputConfig->LookAction,ETriggerEvent::Triggered,this,&AZBPlayerController::Input_Look);
		ZBInputComponent->BindAction(InputConfig->ShiftAction,ETriggerEvent::Started,this,&AZBPlayerController::Input_Sprint_Started);
		ZBInputComponent->BindAction(InputConfig->ShiftAction,ETriggerEvent::Completed,this,&AZBPlayerController::Input_Sprint_Completed);
		ZBInputComponent->BindAction(InputConfig->MenuAction,ETriggerEvent::Started,this,&AZBPlayerController::Input_Menu);
		ZBInputComponent->BindAction(InputConfig->InteractionAction,ETriggerEvent::Started,this,&AZBPlayerController::Input_Interaction);
		ZBInputComponent->BindAction(InputConfig->TargetLockAction,ETriggerEvent::Started,this,&AZBPlayerController::Input_TargetLock);
		
		// 能力输入绑定
		ZBInputComponent->BindAbilityActions(
			InputConfig,
			this,
			&ThisClass::AbilityInputPressed,
			&ThisClass::AbilityInputReleased,
			&ThisClass::AbilityInputHeld);
	}
	
}

void AZBPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	UZBAbilitySystemComponent* ASC = GetASC();
	if (!ASC)
	{
		return;
	}
	ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(InputTag));
	UE_LOG(LogTemp, Log, TEXT("输入按下: %s"), *InputTag.ToString());
}

void AZBPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	UE_LOG(LogTemp, Log, TEXT("输入释放: %s"), *InputTag.ToString());
	
}

void AZBPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	UE_LOG(LogTemp, Log, TEXT("输入长按: %s"), *InputTag.ToString());
}


UZBAbilitySystemComponent* AZBPlayerController::GetASC()
{
	if (ZBAbilitySystemComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("未找到 ASC 组件，开始获取 ASC 组件"));  
		ZBAbilitySystemComponent = Cast<UZBAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	UE_LOG(LogTemp, Warning, TEXT("✓ 找到 ASC 组件：%s"),*ZBAbilitySystemComponent->GetName());
	return ZBAbilitySystemComponent;
}


/**
 * @brief [Input Action] 处理移动输入 (WASD)
 * 基于当前摄像机视角的朝向进行移动，而非角色模型的朝向。
 * * @param InputActionValue 增强输入传入的 2D 向量 (X=Right, Y=Forward)
 */
void AZBPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	// 1. 安全检查：确保 ASC 存在
	// (通常移动不强依赖 GAS，但加上这个检查可以防止在初始化未完成时操作)
	if (!GetASC())return;

	// 2. 获取输入的 2D 向量
	FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	// 3. 计算基于视角的移动方向
	// 获取控制器的旋转（通常等同于摄像机朝向）
	const FRotator Rotation = GetControlRotation();
	// 关键：只保留 Yaw (水平旋转)，将 Pitch (俯仰) 和 Roll 置零
	// 这样可以保证 Forward 向量永远平行于地面，防止角色向天空飞或钻地
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 4. 获取世界空间的方向向量
	// 基于 Yaw 矩阵提取 X轴 (前) 和 Y轴 (右)
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 5. 应用移动输入
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Y 分量对应 W/S (前后)
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		// X 分量对应 A/D (左右)
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
}

/**
 * @brief [Input Action] 处理视角旋转 (鼠标/右摇杆)
 */
void AZBPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	FVector2D LookVector  = InputActionValue.Get<FVector2D>();
	// 应用 Yaw (Z轴旋转) -> 左右看
	AddYawInput(LookVector.X);
	// 应用 Pitch (Y轴旋转) -> 上下看
	AddPitchInput(LookVector.Y);  
}


/**
 * @brief [Input Action] 开始冲刺
 * 当按下 Shift 键时调用。直接应用一个“加速与消耗体力”的 GameplayEffect。
 */
void AZBPlayerController::Input_Sprint_Started()
{
	// 防止重复触发
	if (bIsSprinting) return;

	UZBAbilitySystemComponent* ASC = GetASC();
	if (!ASC) return;
	// 检查配置是否遗漏
	if (!SprintEffectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SprintEffectClass 未配置！"));
		return;
	}
	// 1. 创建上下文 (Context)
	// 上下文包含了谁施放的、施放源是什么等信息，对于伤害计算和标签判定很重要
	FGameplayEffectContextHandle ContextHandle= ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	// 2. 创建 Spec (规格书)
	// 这是 GE 的实例化蓝图。1.f 是 Level (等级)，虽然对于冲刺通常只有 1 级
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(SprintEffectClass,1.f,ContextHandle);

	// 3. 应用 GE 并保存 Handle
	if (SpecHandle.IsValid())
	{
		// ApplyGameplayEffectSpecToSelf：将效果应用给拥有者自己
		// 返回的 Handle 非常重要，它是移除该效果的唯一凭证
		SprintEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		bIsSprinting = true;
	}
	
}

/**
 * @brief [Input Action] 结束冲刺
 * 当松开 Shift 键时调用。移除之前的 GameplayEffect。
 */
void AZBPlayerController::Input_Sprint_Completed()
{
	// 如果本来就没冲刺，直接返回
	if (!bIsSprinting) return;
	
	UZBAbilitySystemComponent* ASC = GetASC();
	if (!ASC) return;

	// 移除指定的 GE 实例
	// Infinite (无限时长) 的 GE 必须手动移除
	if (SprintEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(SprintEffectHandle);
		// 使 Handle 失效，防止野指针或重复移除
		SprintEffectHandle.Invalidate();
	}
	bIsSprinting = false;
	
}

void AZBPlayerController::Input_Interaction()
{
	UE_LOG(LogTemp, Log, TEXT("按下交互键按键"));
}

void AZBPlayerController::Input_TargetLock()
{
	UE_LOG(LogTemp, Log, TEXT("按下锁定目标按键"));
}

void AZBPlayerController::Input_Menu()
{
	UE_LOG(LogTemp, Log, TEXT("按下菜单按键"));
}
