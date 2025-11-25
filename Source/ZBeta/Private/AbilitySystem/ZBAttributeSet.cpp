// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZBAttributeSet.h"
#include "Net/UnrealNetwork.h"

UZBAttributeSet::UZBAttributeSet()
{
	
}

void UZBAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
}

void UZBAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet,Health,OldHealth);
}

void UZBAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet,MaxHealth,OldMaxHealth);
}
