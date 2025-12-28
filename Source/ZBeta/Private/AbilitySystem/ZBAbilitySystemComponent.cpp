// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZBAbilitySystemComponent.h"
#include "AbilitySystem/ZBAbilitySystemLibrary.h"
#include "AbilitySystem/ZBGameplayTags.h"
#include "AbilitySystem/Abilitys/ZBGameplayAbility.h"


/**
 * @brief 处理“输入标签按下”：匹配到的 GA 标记为按下；未激活则尝试激活，已激活则广播 InputPressed（带正确 PredictionKey）
 * @param InputTag 按下的输入标签（如 InputTag.LMB / InputTag.Skill.Q 等）
 * @details
 *  - 使用新 API：AbilitySpec.GetDynamicSpecSourceTags() 进行标签匹配（代替已弃用 DynamicAbilityTags）
 *  - 遍历时使用 FScopedAbilityListLock，保证列表在迭代中的稳定性
 *  - 通过 UAuraAbilitySystemLibrary::AuraGetPredictionKeyFromSpec_Safe 拿到实例上的 PredictionKey
 */
void UZBAbilitySystemComponent::AbilityInputForTagPressed(const FGameplayTag& InputTag)
{
	// 检查 InputTag 是否有效，无效则直接返回
	if (!InputTag.IsValid())return;
	// 锁定能力列表，防止遍历时被修改（多线程安全）
	FScopedAbilityListLock ActiveScopeLoc(*this);
	// 遍历所有可激活的能力
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 【关键点】检查这个 AbilitySpec 是否被赋予了对应的 InputTag
		// 我们在 GrantAbility 时会将 InputTag 放入 DynamicAbilityTags 中
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			// 标记输入状态为“按下”
			AbilitySpecInputPressed(AbilitySpec);

			// 情况 A: 技能已经激活 (例如：蓄力技能正在蓄力中)
			if (AbilitySpec.IsActive())
			{
			 	const  FPredictionKey OriginalPredictionKey= UZBAbilitySystemLibrary::ZBGetPredictionKeyFrom_Safe(AbilitySpec);
				InvokeReplicatedEvent(
					EAbilityGenericReplicatedEvent::InputPressed,// 事件类型：按下
					AbilitySpec.Handle,                         // 该 GA 的句柄
					OriginalPredictionKey);                    // 正确的预测键
			}
			else
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
			
		}
		
	}
}

void UZBAbilitySystemComponent::AbilityInputForTagReleased(const FGameplayTag& InputTag)
{
}

void UZBAbilitySystemComponent::AbilityInputForTagHeld(const FGameplayTag& InputTag)
{
}

void UZBAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartUpAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UZBGameplayAbility* ZBAbility = Cast<UZBGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(ZBAbility->StartupInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FZBGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
		
	}
}

void UZBAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpPassiveAbilities)
{
}
