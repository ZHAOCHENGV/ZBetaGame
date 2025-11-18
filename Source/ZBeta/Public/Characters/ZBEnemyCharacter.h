// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZBCharacterBase.h"
#include "ZBEnemyCharacter.generated.h"

UCLASS()
class ZBETA_API AZBEnemyCharacter : public AZBCharacterBase
{
	GENERATED_BODY()

public:

	AZBEnemyCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

public:


};

