// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Characters/ZBCharacterBase.h"
#include "AbilitySystem/ZBAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"


AZBCharacterBase::AZBCharacterBase()
{
	
	PrimaryActorTick.bCanEverTick = true;


}


void AZBCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZBCharacterBase::InitAbilityActorInfo()
{
	
}

void AZBCharacterBase::InitializeDefaultAttributes()
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);
	check(DefaultDerivedAttributes);
	BindAttributeChangeDelegates();
	
	ApplyEffectToSelf(DefaultPrimaryAttributes,1.f);
	ApplyEffectToSelf(DefaultDerivedAttributes,1.f);


}



/**
 * @brief 对自身应用一个 GameplayEffect（以指定的等级 Level）
 * 
 * @param GamePlayEffectClass 要应用的 GameplayEffect 类
 * @param Level               GameplayEffect 的等级（影响数值，比如 1~100）
 * 
 * @details
 * 功能说明：
 *   - 该函数用于向“自身”应用一个 GameplayEffect，例如：
 *       · 初始化默认属性
 *       · 加 Buff
 *       · 加属性

 *
 * 详细流程：
 *   1. 检查 AbilitySystemComponent 是否有效；
 *   2. 创建一个 EffectContext（用于记录施法者、命中来源等信息）；
 *   3. 用 ASC 创建一个 GameplayEffectSpec（相当于一次“技能伤害/增益数据包”）；
 *   4. 将这个 Spec 应用到自身的 ASC；
 *
 * 注意事项：
 *   - SpecHandle 只是一个“句柄”，内部存的是 TSharedPtr；要获取真实的 Spec 需用 Data.Get()；
 *   - ApplyGameplayEffectSpecToTarget 需要传递的是引用（FGameplayEffectSpec&），所以必须写成 *SpecHandle.Data.Get()；
 *   - Target 可以是自身，也可以是敌人（这里传自身 ASC 表示作用在自己身上）。
 */

void AZBCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GamePlayEffectClass, float Level)
{
	// 检查 AbilitySystemComponent 是否有效，防止空指针崩溃
	check(IsValid(GetAbilitySystemComponent()));
	// 检查 GameplayEffect 类是否有效
	check(GamePlayEffectClass);

	// -------------------------------------------------------------
	// ★ 第 1 步：创建效果上下文（Effect Context）
	//   - Context 记录了“是谁施放的效果”等信息
	//   - Context 可用于计算暴击来源、伤害来源、命中来源等
	// -------------------------------------------------------------
	FGameplayEffectContextHandle ContextHandle= GetAbilitySystemComponent()->MakeEffectContext();
	// 将当前角色作为“效果来源”记录进 EffectContext（非常重要）
	ContextHandle.AddSourceObject(this);
	// -------------------------------------------------------------
	// ★ 第 2 步：根据 GE 类 + 等级 + 上下文 创建一个 Spec（效果规格）
	//   - Spec = 一次“可执行的效果数据包”
	//   - 内部包含：数值、标签、执行逻辑、持续时间等
	// -------------------------------------------------------------
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GamePlayEffectClass,Level,ContextHandle);
	// -------------------------------------------------------------
	// ★ 第 3 步：应用 Spec 到目标（这里目标是自身）
	//   - ApplyGameplayEffectSpecToTarget 需要一个 FGameplayEffectSpec& 引用
	//   - SpecHandle.Data 是一个 TSharedPtr<FGameplayEffectSpec>
	//   - SpecHandle.Data.Get() 取得内部裸指针 FGameplayEffectSpec*
	//   - *SpecHandle.Data.Get() 变成引用 FGameplayEffectSpec&
	// -------------------------------------------------------------
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(),// ← 把 SharedPtr 里的 Spec 取引用出来
		GetAbilitySystemComponent()// ← 目标 ASC（这里是自身 ASC）
		);
}

/**
 * @brief 绑定属性变化委托
 * 通常在 InitAbilityActorInfo 之后调用。
 * 作用：监听 GAS 中 MoveSpeed 属性的数值变化，并同步给 CharacterMovement 组件。
 */
void AZBCharacterBase::BindAttributeChangeDelegates()
{
	if (!AbilitySystemComponent) return;
	const UZBAttributeSet* ZBAttributeSet = Cast<UZBAttributeSet>(AttributeSet);
	if (!ZBAttributeSet) return;

	// 核心逻辑：
	// 1. GetGameplayAttributeValueChangeDelegate: 获取特定属性的变化监听器
	// 2. AddUObject: 绑定回调函数 OnMoveSpeedChanged
	// 只要 MoveSpeed 发生变化（无论是被 GE 修改，还是升级提升），都会触发此回调
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZBAttributeSet->GetMoveSpeedAttribute()).AddUObject(this, &AZBCharacterBase::OnMoveSpeedChanged);
}

/**
 * @brief 移动速度变化回调
 * @param Data 包含了属性的新值 (NewValue) 和旧值 (OldValue)
 */
void AZBCharacterBase::OnMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	// 获取 UE 原生的移动组件
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		// 将 GAS 的数值 (Attribute) 赋值给 物理逻辑 (Component)
		// 这样，当你应用 Sprint GE 时，GAS 里的 MoveSpeed 变大 -> 触发回调 -> MaxWalkSpeed 变大 -> 角色跑得快
		MovementComponent->MaxWalkSpeed = Data.NewValue;
	}
}


void AZBCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AZBCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AZBCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

