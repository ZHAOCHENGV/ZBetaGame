// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZBBlueprintFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

bool UZBBlueprintFunctionLibrary::HasGameplayTagThreadSafe(UAbilitySystemComponent* ASC, FGameplayTag TagToCheck)
{
	if (ASC && TagToCheck.IsValid())
	{
		// 直接调用 ASC 的方法。虽然 ASC 本身不是完全线程安全的，
		// 但读取 Tag 容器（HasMatchingGameplayTag）在动画上下文中通常被视为可接受的操作。
		return ASC->HasMatchingGameplayTag(TagToCheck);
	}
	return false;
}
