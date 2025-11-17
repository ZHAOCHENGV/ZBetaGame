// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZBPlayerState.h"

#include "AbilitySystem/ZBAbilitySystemComponent.h"
#include "AbilitySystem/ZBAttributeSet.h"

AZBPlayerState::AZBPlayerState()
{
	//网络更新频率
	SetNetUpdateFrequency(100.f);
	AbilitySystemComponent = CreateDefaultSubobject<UZBAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UZBAttributeSet>("AttributeSet");
	
}
