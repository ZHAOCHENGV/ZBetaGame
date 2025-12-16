// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ZBInputConfig.generated.h"

class UInputAction;

//BlueprintType 可以在蓝图使用
USTRUCT(BlueprintType)
struct FZBInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere , meta=(DisplayName ="输入操作"))
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditAnywhere , meta=(DisplayName = "输入标签" , Categories = "InputTag"))
	FGameplayTag InputTag = FGameplayTag();
	
};

/**
 * 
 */
UCLASS()
class ZBETA_API UZBInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// ============ 能力输入 ============
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly,Category = "InputConfig|AbilityInputActions", meta=(DisplayName = "能力输入配置组"))
	TArray<FZBInputAction> AbilityInputActions;


	// ============ 原生输入 ============
	UPROPERTY(EditAnywhere,Category = "InputConfig|NativeInputActions", meta=(DisplayName = "移动输入"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly,Category = "InputConfig|NativeInputActions", meta=(DisplayName = "视角输入"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere,Category = "InputConfig|NativeInputActions", meta=(DisplayName = "冲刺输入"))
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly,Category = "InputConfig|NativeInputActions", meta=(DisplayName = "交互输入"))
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditDefaultsOnly,Category = "InputConfig|NativeInputActions", meta=(DisplayName = "锁定输入"))
	TObjectPtr<UInputAction> TargetLockAction;

	UPROPERTY(EditDefaultsOnly,Category = "InputConfig|NativeInputActions", meta=(DisplayName = "菜单输入"))
	TObjectPtr<UInputAction> MenuAction;

	//查找标签的输入动作
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;
	//查找输入动作的标签
	FGameplayTag FindTagForInputAction(const UInputAction* InputAction) const;
};
