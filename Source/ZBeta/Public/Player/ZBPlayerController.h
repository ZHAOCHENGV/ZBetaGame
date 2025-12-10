// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "ZBPlayerController.generated.h"

class UZBAbilitySystemComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UZBInputConfig;
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

	/*
	 * 按键回调函数
	 */
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);

private:
	UPROPERTY(EditAnywhere,  Category = "Input", meta=(DisplayName = "默认输入映射"))
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditAnywhere,  Category = "Input", meta=(DisplayName = "默认资产配置"))
	UZBInputConfig* InputConfig;
	
	UPROPERTY()
	TObjectPtr<UZBAbilitySystemComponent> ZBAbilitySystemComponent;
	UZBAbilitySystemComponent* GetASC();

	
	
	
};
