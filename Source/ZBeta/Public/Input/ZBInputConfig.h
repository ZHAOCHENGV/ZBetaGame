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

	UPROPERTY(EditAnywhere , meta=(DisplayName = "输入标签"))
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
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly, meta=(DisplayName = "输入配置数组"))
	TArray<FZBInputAction> AbilityInputActions;

	//查找标签的输入动作
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;
	//查找输入动作的标签
	FGameplayTag FindTagForInputAction(const UInputAction* InputAction) const;
};
