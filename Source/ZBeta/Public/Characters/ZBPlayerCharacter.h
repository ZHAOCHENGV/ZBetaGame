// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZBCharacterBase.h"
#include "ZBPlayerCharacter.generated.h"

class UGameplayAbility;
class UGameplayCameraComponent;

UCLASS()
class ZBETA_API AZBPlayerCharacter : public AZBCharacterBase
{
	GENERATED_BODY()

public:

	AZBPlayerCharacter();
	

	// @brief 当 Pawn 被新 Controller 占有时调用（服务器 + 本地都会走，但服务器为权威）
	virtual void PossessedBy(AController* NewController) override;

	// @brief 当 PlayerState 在客户端复制完成后回调（仅客户端执行）
	virtual void OnRep_PlayerState() override;


	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	// @brief 初始化 AbilitySystemComponent 的 Owner/Avatar（统一封装）
	virtual void InitAbilityActorInfo() override;


private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> TopDownCameraComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGameplayCameraComponent> TopDownGameplayCameraComponent;



};
