// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ZBAttributeSet.generated.h"



// ----------------------------------------------------------------------------------------------------------------
// 宏定义：自动生成 Getter (获取值), Setter (设置值), Initter (初始化值)
// 用法：MyHealth -> GetMyHealth(), SetMyHealth(), InitMyHealth()
// ----------------------------------------------------------------------------------------------------------------
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

	

	/**
	 * @brief 配置属性的网络复制规则
	 * @note 这是 UE 网络底层的标准接口。在 GAS 中，必须在这里注册所有 Replicated 的 Attribute。
	 * * @param OutLifetimeProps 引擎传入的数组，用于存储复制规则
	 */
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * @brief 在属性值发生“非 GameplayEffect”引起的变更前触发
	 * @details 主要用于钳制（Clamp）通过 SetHealth() 或编辑器面板修改的数值。
	 * @warning 此函数【不会】被 GameplayEffect 触发！GE 修改走 PostGameplayEffectExecute。
	 * * @param Attribute 正在被修改的属性元数据
	 * @param NewValue  即将在内存中生效的新值（引用传递，可被修改）
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**
	 * @brief 当一个 GameplayEffect (GE) 成功应用并修改了 Attribute 后触发
	 * @details 这是 GAS 逻辑最核心的地方。伤害计算、死亡判定、受击反馈都在这里写。
	 * @note 仅在 Server 端触发（除非 GE 预测），且仅针对 Instant 类型的 GE。
	 * * @param Data 包含了 GE 的上下文（谁释放的、谁受击、GE 的原始数值等）
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/**
	 * @brief 在属性值发生【任何】变更后触发
	 * @details 无论是 GE 还是 SetValue 都会触发此函数。
	 * @note 这里的 NewValue 已经是最终值。通常用于处理属性间的联动（如 MaxHealth 变了，Health 需按比例缩放）。
	 * * @param Attribute 被修改的属性
	 * @param OldValue  修改前的值
	 * @param NewValue  修改后的值
	 */
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	

    // ========================================================================================================
    // 1. 主要属性 (Vital & Primary Attributes)
    // 基础三维 (HP/MP/SP) 与 核心属性 (力/智/敏/韧)
    // ========================================================================================================

    // 生命
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes", meta = (DisplayName = "生命"))
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, Health);

    // 法力
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes", meta = (DisplayName = "法力"))
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, Mana);

    // 体力
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Vital Attributes", meta = (DisplayName = "体力"))
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, Stamina);

	// 韧性 (削韧槽)：类似第二条血，被攻击减少，归零后角色进入“踉跄/处决”状态
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Toughness, Category = "Vital Attributes", meta = (DisplayName = "韧性值"))
	FGameplayAttributeData Toughness;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet, Toughness);

    // 力量 (通常增加物理攻击力、负重)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Vital Attributes", meta = (DisplayName = "力量"))
    FGameplayAttributeData Strength;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, Strength);

    // 智力 (通常增加魔法攻击力、最大法力)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Vital Attributes", meta = (DisplayName = "智力"))
    FGameplayAttributeData Intelligence;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, Intelligence);

    // 敏捷 (通常增加暴击率、攻速、移动速度)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "Vital Attributes", meta = (DisplayName = "敏捷"))
    FGameplayAttributeData Dexterity;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, Dexterity);
	
    // --- 主要属性的网络回调声明 ---
    UFUNCTION() virtual void OnRep_Health(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_Stamina(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_Toughness(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_Strength(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_Intelligence(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_Dexterity(const FGameplayAttributeData& OldValue);



	
    // ========================================================================================================
    // 2. 核心属性上限 (Max Attributes)
    // 决定了 HP/MP/SP 的上限值
    // ========================================================================================================

    // 最大生命值
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Max Attributes", meta = (DisplayName = "最大生命值"))
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, MaxHealth);

    // 最大法力值
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Max Attributes", meta = (DisplayName = "最大法力值"))
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, MaxMana);

    // 最大体力值
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Max Attributes", meta = (DisplayName = "最大体力值"))
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, MaxStamina);

	// 最大韧性值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxToughness, Category = "Max Attributes", meta = (DisplayName = "最大韧性"))
	FGameplayAttributeData MaxToughness;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet, MaxToughness);

    // --- 上限属性的网络回调声明 ---
    UFUNCTION() virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_MaxToughness(const FGameplayAttributeData& OldValue);


	
    // ========================================================================================================
    // 3. 抗性 (Resistance)
    // 减伤与硬直抵抗
    // ========================================================================================================
	
    // 物理抗性 (即护甲，用于计算物理减伤)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes", meta = (DisplayName = "物理抗性(护甲)"))
    FGameplayAttributeData PhysicalResistance;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, PhysicalResistance);

    // 魔法抗性 (用于计算魔法减伤)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicResistance, Category = "Resistance Attributes", meta = (DisplayName = "魔法抗性"))
    FGameplayAttributeData MagicResistance;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, MagicResistance);

    // --- 抗性属性的网络回调声明 ---
    UFUNCTION() virtual void OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_MagicResistance(const FGameplayAttributeData& OldValue);


	
    // ========================================================================================================
    // 4. 回复效率 (Regeneration Rates)
    // 每秒自动恢复的数值
    // ========================================================================================================

    // 生命回复率
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegenRate, Category = "Regen Attributes", meta = (DisplayName = "生命回复率"))
    FGameplayAttributeData HealthRegenRate;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, HealthRegenRate);

    // 法力回复率
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegenRate, Category = "Regen Attributes", meta = (DisplayName = "法力回复率"))
    FGameplayAttributeData ManaRegenRate;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, ManaRegenRate);

    // 体力回复率
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaRegenRate, Category = "Regen Attributes", meta = (DisplayName = "体力回复率"))
    FGameplayAttributeData StaminaRegenRate;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, StaminaRegenRate);

	//韧性回复率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ToughnessRegenRate, Category = "Regen Attributes", meta = (DisplayName = "韧性恢复率"))
	FGameplayAttributeData ToughnessRegenRate;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet, ToughnessRegenRate);

    // --- 回复属性的网络回调声明 ---
    UFUNCTION() virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_ManaRegenRate(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_ToughnessRegenRate(const FGameplayAttributeData& OldValue);



	// ========================================================================================================
	// 5. 体力消耗 (StaminaCost)
	// 冲刺/闪避体力消耗率
	// ========================================================================================================
	
	// 冲刺体力消耗倍率 (默认为1.0，越小消耗越少)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DodgeStaminaCostMultiplier, Category = "Stats Attributes", meta = (DisplayName = "体力消耗倍率"))
	FGameplayAttributeData DodgeStaminaCostMultiplier;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet, DodgeStaminaCostMultiplier);

	// 闪避体力消耗倍率 (默认为1.0，越小消耗越少)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SprintStaminaCostMultiplier, Category = "Stats Attributes", meta = (DisplayName = "体力消耗倍率"))
	FGameplayAttributeData SprintStaminaCostMultiplier;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet, SprintStaminaCostMultiplier);
	
	// --- 回复属性的网络回调声明 ---
	UFUNCTION() virtual void OnRep_SprintStaminaCostMultiplier(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_DodgeStaminaCostMultiplier(const FGameplayAttributeData& OldValue);


	
    // ========================================================================================================
    // 6. 吸取/偷取 (Vamp/Steal)
    // 攻击命中时回复自身的比率
    // ========================================================================================================

    // 生命吸取率 (吸血)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthSteal, Category = "Combat Attributes", meta = (DisplayName = "生命吸取率"))
    FGameplayAttributeData HealthSteal;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, HealthSteal);

    // 法力吸取率 (吸蓝)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaSteal, Category = "Combat Attributes", meta = (DisplayName = "法力吸取率"))
    FGameplayAttributeData ManaSteal;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, ManaSteal);

    // 体力吸取率(吸耐力)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaSteal, Category = "Combat Attributes", meta = (DisplayName = "体力吸取率"))
    FGameplayAttributeData StaminaSteal;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, StaminaSteal);

	
	// 暴击率 (百分比，例如 0.5 代表 50%)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category = "Combat Attributes", meta = (DisplayName = "暴击率"))
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet, CriticalChance);

	// 暴击伤害 (倍率，例如 1.5 代表 150% 伤害，2.0 代表 200% 伤害)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalDamage, Category = "Combat Attributes", meta = (DisplayName = "暴击伤害"))
	FGameplayAttributeData CriticalDamage;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet, CriticalDamage);

    // --- 吸取属性的网络回调声明 ---
    UFUNCTION() virtual void OnRep_HealthSteal(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_ManaSteal(const FGameplayAttributeData& OldValue);
    UFUNCTION() virtual void OnRep_StaminaSteal(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_CriticalChance(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_CriticalDamage(const FGameplayAttributeData& OldValue);
	

	
    // ========================================================================================================
    // 7. 移速、负重、
    // ========================================================================================================
	
	// 移动速度 (修正 CharacterMovement 的速度)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category = "Stats Attributes", meta = (DisplayName = "移动速度"))
    FGameplayAttributeData MoveSpeed;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, MoveSpeed);
	
    // 最大装备负重 (决定是否超重)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEquipmentLoad, Category = "Stats Attributes", meta = (DisplayName = "最大装备负重"))
    FGameplayAttributeData MaxEquipmentLoad;
    ATTRIBUTE_ACCESSORS(UZBAttributeSet, MaxEquipmentLoad);

	// --- 状态属性的网络回调声明 ---
	UFUNCTION() virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_MaxEquipmentLoad(const FGameplayAttributeData& OldValue);

	
	// ========================================================================================================
	// 8. 元属性（传值属性）
	// ========================================================================================================
	UPROPERTY(BlueprintReadOnly,  Category = "Meta Attributes", meta = (DisplayName = "受到的伤害数值"))
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly,  Category = "Meta Attributes", meta = (DisplayName = "增加的经验值"))
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UZBAttributeSet, IncomingXP);





private:
	
};

