// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZBBlueprintFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

bool UZBBlueprintFunctionLibrary::HasGameplayTagThreadSafe(UAbilitySystemComponent* ASC, FGameplayTag TagToCheck)
{
	if (ASC && TagToCheck.IsValid())
	{
		return ASC->HasMatchingGameplayTag(TagToCheck);
	}
	return false;
}
