// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/ZBInputConfig.h"
#include "InputAction.h"

const UInputAction* UZBInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FZBInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
		UE_LOG(LogTemp, Error, TEXT("在输入配置[%s]中，找不到与输入标签[%s]对应的输入操作"), *GetNameSafe(this),*InputTag.ToString());
		return nullptr;
}

FGameplayTag UZBInputConfig::FindTagForInputAction(const UInputAction* InputAction) const
{
	for (const FZBInputAction& Action : AbilityInputActions)
	{
		if (Action.InputTag.IsValid() && Action.InputAction == InputAction)
		{
			return Action.InputTag;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("在输入配置[%s]中，找不到与输入操作[%s]对应的输入标签"), *GetNameSafe(this),*GetNameSafe(InputAction));
	return FGameplayTag();
}
