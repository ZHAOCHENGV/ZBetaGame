// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayPrediction.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZBAbilitySystemLibrary.generated.h"

struct FGameplayAbilitySpec;

/**
 * 
 */
UCLASS()
class ZBETA_API UZBAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * @brief 安全获取技能的预测键 (Prediction Key)
	 * * @param Spec 技能规格书
	 * @return FPredictionKey 有效的预测键，如果未激活则返回无效键
	 */
	UFUNCTION()
	static FPredictionKey ZBGetPredictionKeyFrom_Safe(const FGameplayAbilitySpec& Spec);
};
