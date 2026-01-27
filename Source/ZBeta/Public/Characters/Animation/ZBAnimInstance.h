// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "ZBAnimInstance.generated.h"


/**
 * 本项目所使用的基础动画类
 */
UCLASS(Config=Game)
class ZBETA_API UZBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UZBAnimInstance(const FObjectInitializer& ObjectInitializer);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags", meta=(DisplayName = "游戏玩法标签属性映射表"))
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};
