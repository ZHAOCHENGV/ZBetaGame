// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ZBAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZBETA_API UZBAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityInputForTagPressed(const FGameplayTag& InputTag);
	void AbilityInputForTagReleased(const FGameplayTag& InputTag);
	void AbilityInputForTagHeld(const FGameplayTag& InputTag);	
};
