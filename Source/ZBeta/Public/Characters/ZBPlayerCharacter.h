// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZBCharacterBase.h"
#include "ZBPlayerCharacter.generated.h"

UCLASS()
class ZBETA_API AZBPlayerCharacter : public AZBCharacterBase
{
	GENERATED_BODY()

public:

	AZBPlayerCharacter();
	

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;


	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;


};
