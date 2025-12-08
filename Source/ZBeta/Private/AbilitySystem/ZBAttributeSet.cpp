// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZBAttributeSet.h"
#include "Net/UnrealNetwork.h"

UZBAttributeSet::UZBAttributeSet()
{

}	

void UZBAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 1. 主要属性
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, Toughness, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, Strength, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);

    // 2. 属性上限
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, MaxToughness, COND_None, REPNOTIFY_Always);

    // 3. 抗性
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);

    // 4. 回复
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, ManaRegenRate, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, StaminaRegenRate, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, ToughnessRegenRate, COND_None, REPNOTIFY_Always);

    //5. 体力消耗率
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet,DodgeStaminaCostMultiplier, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet,SprintStaminaCostMultiplier, COND_None, REPNOTIFY_Always);
    
    // 6. 吸取
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, HealthSteal, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, ManaSteal, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, StaminaSteal, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);

    // 7. 状态
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UZBAttributeSet, MaxEquipmentLoad, COND_None, REPNOTIFY_Always);
}

void UZBAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);
}

void UZBAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);
}

void UZBAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);
}


// ========================================================================================
// 网络回调实现 (Replication Notifies)
// 宏 GAMEPLAYATTRIBUTE_REPNOTIFY 负责处理预测回滚和旧值比对
// ========================================================================================

// --- 主要属性 ---
void UZBAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, Health, OldValue);
}

void UZBAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, Mana, OldValue);
}

void UZBAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, Stamina, OldValue);
}

void UZBAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, Strength, OldValue);
}

void UZBAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, Intelligence, OldValue);
}

void UZBAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, Dexterity, OldValue);
}

// --- 属性上限 ---
void UZBAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, MaxHealth, OldValue);
}

void UZBAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, MaxMana, OldValue);
}

void UZBAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, MaxStamina, OldValue);
}

// --- 抗性 ---
void UZBAttributeSet::OnRep_Toughness(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, Toughness, OldValue);
}

void UZBAttributeSet::OnRep_MaxToughness(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, MaxToughness, OldValue);
}

void UZBAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, PhysicalResistance, OldValue);
}

void UZBAttributeSet::OnRep_MagicResistance(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, MagicResistance, OldValue);
}

// --- 回复 ---
void UZBAttributeSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, HealthRegenRate, OldValue);
}

void UZBAttributeSet::OnRep_ManaRegenRate(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, ManaRegenRate, OldValue);
}

void UZBAttributeSet::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, StaminaRegenRate, OldValue);
}

void UZBAttributeSet::OnRep_ToughnessRegenRate(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, ToughnessRegenRate, OldValue);
}

// --- 消耗 ---
void UZBAttributeSet::OnRep_DodgeStaminaCostMultiplier(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, DodgeStaminaCostMultiplier, OldValue);
}

void UZBAttributeSet::OnRep_SprintStaminaCostMultiplier(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, SprintStaminaCostMultiplier, OldValue);
}

// --- 吸取 ---
void UZBAttributeSet::OnRep_HealthSteal(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, HealthSteal, OldValue);
}

void UZBAttributeSet::OnRep_ManaSteal(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, ManaSteal, OldValue);
}

void UZBAttributeSet::OnRep_StaminaSteal(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, StaminaSteal, OldValue);
}

// --- 状态 ---

void UZBAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, CriticalChance, OldValue);
}

void UZBAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, CriticalDamage, OldValue);
}

void UZBAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, MoveSpeed, OldValue);
}

void UZBAttributeSet::OnRep_MaxEquipmentLoad(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UZBAttributeSet, MaxEquipmentLoad, OldValue);
}

