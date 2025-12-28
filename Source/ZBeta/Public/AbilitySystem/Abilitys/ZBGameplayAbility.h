// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ZBGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ZBETA_API UZBGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "启动输入标签", Categories = "InputTag"))
	FGameplayTag StartupInputTag;
};
