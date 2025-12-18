// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"  // 添加这个头文件
#include "ZBPlayerController.generated.h"

struct FActiveGameplayEffectHandle;
class UGameplayEffect;
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


	/**
	 * @brief 能力输入按下回调
	 * @param InputTag 触发的输入标签
	 * 
	 * 当能力相关的输入按下时调用，尝试激活对应的 Ability。
	 */
	void AbilityInputPressed(FGameplayTag InputTag);
	/**
	 * @brief 能力输入释放回调
	 * @param InputTag 触发的输入标签
	 * 
	 * 当能力相关的输入释放时调用，用于处理蓄力攻击等。
	 */
	void AbilityInputReleased(FGameplayTag InputTag);
	/**
	 * @brief 输入持续按住时调用
	 * @param InputTag 触发的输入标签
	 */
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
	/**
	 * @brief [Input Action] 开始冲刺
	 */
	void Input_Sprint_Started();
	/**
	 * @brief [Input Action] 结束冲刺
	 */
	void Input_Sprint_Completed();
	void Input_Interaction();
	void Input_TargetLock();
	void Input_Menu();

	//是否正在冲刺
	bool bIsSprinting = false;

	// 冲刺效果类
	UPROPERTY(EditAnywhere, Category = "Abilites", meta = (DisplayName = "冲刺效果"))
	TSubclassOf<UGameplayEffect> SprintEffectClass;

	// 冲刺效果句柄，用于移除效果
	FActiveGameplayEffectHandle SprintEffectHandle;
	
	




	
	
	
};
