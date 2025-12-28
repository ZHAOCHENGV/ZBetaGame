// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZBAbilitySystemLibrary.h"

#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"


/**
 * @brief 安全获取技能的预测键 (Prediction Key)
 * * 遍历 Spec 下的所有实例，寻找有效的预测键。
 * 修复了访问已废弃变量 Spec.ActivationInfo 的问题。
 * * @param Spec 技能规格书
 * @return FPredictionKey 有效的预测键，如果未激活则返回无效键
 */
FPredictionKey UZBAbilitySystemLibrary::ZBGetPredictionKeyFrom_Safe(const FGameplayAbilitySpec& Spec)
{
	// 1. 检查主实例 (InstancedPerActor 策略通常走这里)
	if (UGameplayAbility* Primary = Spec.GetPrimaryInstance())
	{
		return Primary->GetCurrentActivationInfo().GetActivationPredictionKey();
	}

	// 2. 检查所有实例副本 (InstancedPerExecution 策略通常走这里)
	// GetAbilityInstances 返回的是数组副本，直接遍历
	TArray<UGameplayAbility*> Instances= Spec.GetAbilityInstances();
	for (UGameplayAbility* Instance : Instances)
	{
		if (!Instance) continue;
		const FPredictionKey Key  = Instance->GetCurrentActivationInfo().GetActivationPredictionKey();
		if (Key.IsValidKey())
		{
			return Key;
		}
	}
	
	// 3. 如果没有实例或实例都未激活，直接返回空键
	return FPredictionKey();
	// [Fix]: 不要使用 Spec.ActivationInfo，它已被废弃且在实例化技能中无效
	//return Spec.ActivationInfo.GetActivationPredictionKey();
}
