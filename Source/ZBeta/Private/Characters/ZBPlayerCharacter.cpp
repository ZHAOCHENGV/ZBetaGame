// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ZBPlayerCharacter.h"



AZBPlayerCharacter::AZBPlayerCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;
}

void AZBPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void AZBPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo();
}


void AZBPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZBPlayerCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
}


void AZBPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AZBPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

