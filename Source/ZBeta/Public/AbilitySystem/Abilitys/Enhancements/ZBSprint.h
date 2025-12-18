// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/ZBGameplayAbility.h"
#include "ZBSprint.generated.h"

/**
 * 
 */
UCLASS()
class ZBETA_API UZBSprint : public UZBGameplayAbility
{
	GENERATED_BODY()

public:
	UZBSprint();

	UPROPERTY(EditDefaultsOnly, Category = "效果" , meta=(DisplayName = "冲刺效果"))
	TSubclassOf<UGameplayEffect> SprintSpeedEffectClass;

	FActiveGameplayEffectHandle SprintSpeedEffectHandle;

	
	
};
