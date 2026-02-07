// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZBBlueprintFunctionLibrary.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ZBETA_API UZBBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// 关键点：标记为 BlueprintThreadSafe，使其能在动画线程中被调用
	// 同时也标记为 Pure，这样没有执行引脚，使用更方便
	UFUNCTION(BlueprintPure, Category = "GAS|Animation", meta=(BlueprintThreadSafe))
	static bool HasGameplayTagThreadSafe(UAbilitySystemComponent* ASC, FGameplayTag TagToCheck);

	
};
