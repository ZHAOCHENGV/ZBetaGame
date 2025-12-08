// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZBPlayerController.generated.h"

class UZBAbilitySystemComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class ZBETA_API AZBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZBPlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere,  Category = "Input", meta=(DisplayName = "默认输入映射"))
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditAnywhere,  Category = "Input|Action", meta=(DisplayName = "移动输入"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere,  Category = "Input|Action", meta=(DisplayName = "冲刺输入"))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere,  Category = "Input|Action", meta=(DisplayName = "闪避输入"))
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere,  Category = "Input|Action", meta=(DisplayName = "格挡输入"))
	TObjectPtr<UInputAction> BlockAction;

	UPROPERTY(EditAnywhere,  Category = "Input|Action", meta=(DisplayName = "普攻输入"))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere,  Category = "Input|Action", meta=(DisplayName = "交互输入"))
	TObjectPtr<UInputAction> InteractionAction;
	
	UPROPERTY(EditAnywhere,  Category = "Input|Action", meta=(DisplayName = "锁定目标输入"))
	TObjectPtr<UInputAction> TargetLockAction;

	UPROPERTY(EditAnywhere,  Category = "Input|Action", meta=(DisplayName = "菜单UI输入"))
	TObjectPtr<UInputAction> MenusAction;

	void Move(const FInputActionValue & InputActionValue);

	UPROPERTY()
	TObjectPtr<UZBAbilitySystemComponent> ZBAbilitySystemComponent;
	UZBAbilitySystemComponent* GetASC();

	
	
	
};
