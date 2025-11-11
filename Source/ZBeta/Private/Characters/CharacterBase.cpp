// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Characters/CharacterBase.h"



ACharacterBase::ACharacterBase()
{
	
	PrimaryActorTick.bCanEverTick = true;
}


void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

