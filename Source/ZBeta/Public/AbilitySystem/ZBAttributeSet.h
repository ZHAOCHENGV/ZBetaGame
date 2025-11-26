// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ZBAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class ZBETA_API UZBAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UZBAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*
	* 主要属性
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category = "Vital Attributes" , meta=(DisplayName = "生命"))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet,Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Mana, Category = "Vital Attributes" , meta=(DisplayName = "法力"))
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet,Mana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Stamina, Category = "Vital Attributes", meta=(DisplayName = "体力"))
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet,Stamina);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Strength, Category = "Vital Attributes", meta=(DisplayName = "力量"))
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet,Strength);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Intelligence, Category = "Vital Attributes", meta=(DisplayName = "智力"))
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet,Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Dexterity, Category = "Vital Attributes", meta=(DisplayName = "敏捷"))
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet,Dexterity);





	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

};

