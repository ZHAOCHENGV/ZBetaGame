// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCale/MMC_MaxStamina.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
	
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	return Super::CalculateBaseMagnitude_Implementation(Spec);
}
