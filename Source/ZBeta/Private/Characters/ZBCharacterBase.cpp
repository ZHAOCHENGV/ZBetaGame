// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Characters/ZBCharacterBase.h"

#include "AbilitySystem/ZBAbilitySystemComponent.h"
#include "AbilitySystem/ZBAttributeSet.h"


AZBCharacterBase::AZBCharacterBase()
{
	
	PrimaryActorTick.bCanEverTick = true;


}


void AZBCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AZBCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AZBCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AZBCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

