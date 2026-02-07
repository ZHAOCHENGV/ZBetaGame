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
	UFUNCTION(BlueprintPure, Category = "GAS|Animation", meta=(BlueprintThreadSafe))
	static bool HasGameplayTagThreadSafe(UAbilitySystemComponent* ASC, FGameplayTag TagToCheck);

	
};
