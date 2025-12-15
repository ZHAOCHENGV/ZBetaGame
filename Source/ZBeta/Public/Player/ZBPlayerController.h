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
	 * 能力按键回调函数
	 */
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);

private:
		
	UPROPERTY()
	TObjectPtr<UZBAbilitySystemComponent> ZBAbilitySystemComponent;
	UZBAbilitySystemComponent* GetASC();
	
	UPROPERTY(EditAnywhere,  Category = "Input", meta=(DisplayName = "默认输入映射"))
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditAnywhere,  Category = "Input", meta=(DisplayName = "默认资产配置"))
	UZBInputConfig* InputConfig;


	/*
	 * 输入操作函数
	 */

	
	/**
	 * @brief [Input Action] 处理角色的移动输入（WASD / 左摇杆）。
	 * * @param InputActionValue 增强输入系统传递的输入值，包含 X（左右）和 Y（前后）分量。
	 */
	void Input_Move(const FInputActionValue& InputActionValue);
	
	/**
	 * @brief [Input Action] 处理角色的视角旋转输入（鼠标移动 / 右摇杆）。
	 * * @param InputActionValue 增强输入系统传递的输入值，X对应Yaw，Y对应Pitch。
	 */
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Sprint_Started();
	void Input_Sprint_Completed();
	void Input_Interaction();
	void Input_TargetLock();
	void Input_Menu();

	//是否正在冲刺
	bool bIsSprinting = false;
	




	
	
	
};
