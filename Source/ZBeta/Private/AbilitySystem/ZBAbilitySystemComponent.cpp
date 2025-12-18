// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZBAbilitySystemComponent.h"

#include "AbilitySystem/Abilitys/ZBGameplayAbility.h"

void UZBAbilitySystemComponent::AbilityInputForTagPressed(const FGameplayTag& InputTag)
{
	// 检查 InputTag 是否有效，无效则直接返回
	if (!InputTag.IsValid())return;
	// 锁定能力列表，防止遍历时被修改（多线程安全）
	FScopedAbilityListLock ActiveScopeLoc(*this);
	// 遍历所有可激活的能力
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (IsValid(AbilitySpec.Ability))
		{
			UZBGameplayAbility* ZBAbility = Cast<UZBGameplayAbility>(AbilitySpec.Ability);
			if (ZBAbility && ZBAbility->InputTag.MatchesTagExact(InputTag))
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
