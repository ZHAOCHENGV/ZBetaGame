/* --- 完整文件代码 --- */
// Fill out your copyright notice in the Description page of Project Settings.
#include "ZBGameplayTags.h"
#include "GameplayTagsManager.h"


// ==================== 单例定义 ====================

/**
 * @brief 全局静态单例实例定义
 * @note 
 *   这里定义了 FZBGameplayTags 的唯一实例。
 *   C++ 静态存储期保证这个对象在程序启动时自动构造，
 *   并在程序结束时自动析构。
 */
FZBGameplayTags FZBGameplayTags::GameplayTags;


// ==================== 初始化方法实现 ====================

void FZBGameplayTags::InitializeNativeTags()
{
	// 🔧 修改 - 通过静态成员 GameplayTags 访问所有非静态成员变量
	// 这是访问单例实例的正确方式
	FZBGameplayTags& Tags = GameplayTags;

	// ===== 属性相关 Tags 初始化 =====
	
	// 🔧 生命值系列
	Tags.AddTag(
		Tags.Attributes_Vital_Health,
		FName(TEXT("Attributes.Vital.Health")),
		TEXT("当前生命值，范围 [0, MaxHealth]")
	);
	
	Tags.AddTag(
		Tags.Attributes_Vital_Mana,
		FName(TEXT("Attributes.Vital.Mana")),
		TEXT("当前法力值，范围 [0, MaxMana]，用于法术消耗")
	);
	
	Tags.AddTag(
		Tags.Attributes_Vital_Stamina,
		FName(TEXT("Attributes.Vital.Stamina")),
		TEXT("当前耐力值，范围 [0, MaxStamina]，用于翻滚和冲刺消耗")
	);
	
	Tags.AddTag(
		Tags.Attributes_Vital_Toughness,
		FName(TEXT("Attributes.Vital.Toughness")),
		TEXT("当前韧性值，防止被打断，范围 [0, MaxToughness]")
	);
	
	Tags.AddTag(
		Tags.Attributes_Vital_Strength,
		FName(TEXT("Attributes.Vital.Strength")),
		TEXT("力量属性，影响物理伤害倍数和最大负载")
	);
	
	Tags.AddTag(
		Tags.Attributes_Vital_Intelligence,
		FName(TEXT("Attributes.Vital.Intelligence")),
		TEXT("智力属性，影响魔法伤害倍数和法力上限")
	);
	
	Tags.AddTag(
		Tags.Attributes_Vital_Dexterity,
		FName(TEXT("Attributes.Vital.Dexterity")),
		TEXT("敏捷属性，影响移动速度、暴击率和闪避效果")
	);

	// 🔧 最大值系列
	Tags.AddTag(
		Tags.Attributes_Max_MaxHealth,
		FName(TEXT("Attributes.Max.MaxHealth")),
		TEXT("最大生命值上限")
	);
	
	Tags.AddTag(
		Tags.Attributes_Max_MaxMana,
		FName(TEXT("Attributes.Max.MaxMana")),
		TEXT("最大法力值上限")
	);
	
	Tags.AddTag(
		Tags.Attributes_Max_MaxStamina,
		FName(TEXT("Attributes.Max.MaxStamina")),
		TEXT("最大耐力值上限")
	);
	
	Tags.AddTag(
		Tags.Attributes_Max_Toughness,
		FName(TEXT("Attributes.Max.Toughness")),
		TEXT("最大韧性值上限")
	);

	// 🔧 抗性系列
	Tags.AddTag(
		Tags.Attributes_Resistance_Physical,
		FName(TEXT("Attributes.Resistance.Physical")),
		TEXT("物理伤害抗性，百分比值 [0-100]")
	);
	
	Tags.AddTag(
		Tags.Attributes_Resistance_Magical,
		FName(TEXT("Attributes.Resistance.Magical")),
		TEXT("魔法伤害抗性，百分比值 [0-100]")
	);

	// 🔧 恢复速率系列
	Tags.AddTag(
		Tags.Attributes_RegenRate_Health,
		FName(TEXT("Attributes.RegenRate.Health")),
		TEXT("生命值恢复速率，单位：点/秒，仅在非战斗状态触发")
	);
	
	Tags.AddTag(
		Tags.Attributes_RegenRate_Mana,
		FName(TEXT("Attributes.RegenRate.Mana")),
		TEXT("法力值恢复速率，单位：点/秒")
	);
	
	Tags.AddTag(
		Tags.Attributes_RegenRate_Stamina,
		FName(TEXT("Attributes.RegenRate.Stamina")),
		TEXT("耐力值恢复速率，单位：点/秒，战斗中触发较快")
	);
	
	Tags.AddTag(
		Tags.Attributes_RegenRate_Toughness,
		FName(TEXT("Attributes.RegenRate.Toughness")),
		TEXT("韧性值恢复速率，单位：点/秒")
	);

	// 🔧 耐力成本倍数系列
	Tags.AddTag(
		Tags.Attributes_StaminaCostMultiplier_Dodge,
		FName(TEXT("Attributes.StaminaCostMultiplier.Dodge")),
		TEXT("闪避耐力消耗倍数，默认 1.0，> 1 表示消耗增加")
	);
	
	Tags.AddTag(
		Tags.Attributes_StaminaCostMultiplier_Sprint,
		FName(TEXT("Attributes.StaminaCostMultiplier.Sprint")),
		TEXT("冲刺耐力消耗倍数，默认 1.0")
	);

	// 🔧 吸取系列
	Tags.AddTag(
		Tags.Attributes_Steal_Health,
		FName(TEXT("Attributes.Steal.Health")),
		TEXT("生命吸取百分比，攻击命中时从伤害中恢复，范围 [0-100]")
	);
	
	Tags.AddTag(
		Tags.Attributes_Steal_Mana,
		FName(TEXT("Attributes.Steal.Mana")),
		TEXT("法力吸取百分比，范围 [0-100]")
	);
	
	Tags.AddTag(
		Tags.Attributes_Steal_Stamina,
		FName(TEXT("Attributes.Steal.Stamina")),
		TEXT("耐力吸取百分比，范围 [0-100]")
	);

	// 🔧 暴击系列
	Tags.AddTag(
		Tags.Attributes_Critical_Chance,
		FName(TEXT("Attributes.Critical.Chance")),
		TEXT("暴击率，百分比 [0-100]，达到此概率时伤害翻倍")
	);
	
	Tags.AddTag(
		Tags.Attributes_Critical_Damage,
		FName(TEXT("Attributes.Critical.Damage")),
		TEXT("暴击伤害倍数，1.0 = 100% 额外伤害，如 1.5 = 150% 额外伤害")
	);

	// 🔧 统计数据系列
	Tags.AddTag(
		Tags.Attributes_Stats_MoveSpeed,
		FName(TEXT("Attributes.Stats.MoveSpeed")),
		TEXT("移动速度，单位：cm/s，与 Character Movement MaxWalkSpeed 联动")
	);
	
	Tags.AddTag(
		Tags.Attributes_Stats_MaxEquipmentLoad,
		FName(TEXT("Attributes.Stats.MaxEquipmentLoad")),
		TEXT("最大装备负载，单位：kg，超过此值无法翻滚（进入 Over 负重状态）")
	);

	// 🔧 元数据系列
	Tags.AddTag(
		Tags.Attributes_Meta_IncomingXP,
		FName(TEXT("Attributes.Meta.IncomingXP")),
		TEXT("接收的经验值，临时存储用于 ModifyAttribute 回调触发")
	);

	// ===== 输入相关 Tags 初始化 =====
	
	// 🔧 攻击输入
	Tags.AddTag(
		Tags.InputTag_Attack_Main,
		FName(TEXT("InputTag.Attack.Main")),
		TEXT("普通攻击输入 - 短按轻攻击，长按重攻击")
	);

	// 🔧 防守/机动输入
	Tags.AddTag(
		Tags.InputTag_Dodge,
		FName(TEXT("InputTag.Dodge")),
		TEXT("闪避输入 - 消耗耐力并获得无敌帧")
	);
	
	Tags.AddTag(
		Tags.InputTag_Block,
		FName(TEXT("InputTag.Block")),
		TEXT("格挡输入 - 按住时持续格挡并消耗耐力")
	);
	
	Tags.AddTag(
		Tags.InputTag_Sprint,
		FName(TEXT("InputTag.Sprint")),
		TEXT("冲刺输入 - 持续消耗耐力加快移动")
	);

	// 🔧 技能输入
	Tags.AddTag(
		Tags.InputTag_Rune_1,
		FName(TEXT("InputTag.Rune.1")),
		TEXT("符文技能槽位 1 输入")
	);
	
	Tags.AddTag(
		Tags.InputTag_Rune_2,
		FName(TEXT("InputTag.Rune.2")),
		TEXT("符文技能槽位 2 输入")
	);
	
	Tags.AddTag(
		Tags.InputTag_Rune_3,
		FName(TEXT("InputTag.Rune.3")),
		TEXT("符文技能槽位 3 输入")
	);
	
	Tags.AddTag(
		Tags.InputTag_Rune_4,
		FName(TEXT("InputTag.Rune.4")),
		TEXT("符文技能槽位 4 输入")
	);

	// 🔧 交互/UI 输入
	Tags.AddTag(
		Tags.InputTag_Interaction,
		FName(TEXT("InputTag.Interaction")),
		TEXT("交互输入 - 靠近 NPC、宝箱、传送点时激活")
	);
	
	Tags.AddTag(
		Tags.InputTag_TargetLock,
		FName(TEXT("InputTag.TargetLock")),
		TEXT("锁定目标输入 - 激活/关闭锁定目标模式")
	);
	
	Tags.AddTag(
		Tags.InputTag_Menus,
		FName(TEXT("InputTag.Menus")),
		TEXT("菜单输入 - 打开/关闭主菜单、背包等 UI")
	);
	
	Tags.AddTag(
		Tags.InputTag_Consumable,
		FName(TEXT("InputTag.Consumable")),
		TEXT("药水/消耗品输入 - 快速使用背包中的消耗品")
	);

	// ===== 能力标识 Tags 初始化 =====
	
	// 🔧 攻击能力
	Tags.AddTag(
		Tags.Ability_Attack_Light,
		FName(TEXT("Ability.Attack.Light")),
		TEXT("轻攻击能力标签 - 便于集中取消或阻止")
	);
	
	Tags.AddTag(
		Tags.Ability_Attack_Heavy,
		FName(TEXT("Ability.Attack.Heavy")),
		TEXT("重攻击能力标签 - 消耗更多资源但伤害更高")
	);

	// 🔧 防守/机动能力
	Tags.AddTag(
		Tags.Ability_Dodge,
		FName(TEXT("Ability.Dodge")),
		TEXT("闪避能力标签 - 标记所有闪避动作")
	);
	
	Tags.AddTag(
		Tags.Ability_Block,
		FName(TEXT("Ability.Block")),
		TEXT("格挡能力标签 - 标记所有格挡相关的 GA")
	);
	
	Tags.AddTag(
		Tags.Ability_Parry,
		FName(TEXT("Ability.Parry")),
		TEXT("招架能力标签 - 精确格挡，成功后无伤并反击")
	);

	// 🔧 技能能力
	Tags.AddTag(
		Tags.Ability_Rune,
		FName(TEXT("Ability.Rune")),
		TEXT("符文技能父标签 - 所有符文技能都应包含此标签")
	);

	// ===== 角色状态 Tags 初始化 =====
	
	// 🔧 帧状态
	Tags.AddTag(
		Tags.State_IFrame,
		FName(TEXT("State.IFrame")),
		TEXT("无敌帧状态 - 所有伤害免疫")
	);
	
	Tags.AddTag(
		Tags.State_HyperArmor,
		FName(TEXT("State.HyperArmor")),
		TEXT("霸体状态 - 不被打断或击飞，但仍受伤害")
	);
	
	Tags.AddTag(
		Tags.State_Blocking,
		FName(TEXT("State.Blocking")),
		TEXT("格挡中状态 - 角色正在主动格挡")
	);
	
	Tags.AddTag(
		Tags.State_Staggered,
		FName(TEXT("State.Staggered")),
		TEXT("硬直状态 - 受击后无法执行任何操作")
	);
	
	Tags.AddTag(
		Tags.State_GuardBroken,
		FName(TEXT("State.GuardBroken")),
		TEXT("破防状态 - 格挡值达到上限，防守能力下降 50%")
	);
	
	Tags.AddTag(
		Tags.State_CanCancel,
		FName(TEXT("State.CanCancel")),
		TEXT("可取消窗口状态 - 当前动作可被打断或取消")
	);
	
	Tags.AddTag(
		Tags.State_HitWindowActive,
		FName(TEXT("State.HitWindowActive")),
		TEXT("命中窗口激活状态 - 攻击的有效伤害帧已打开")
	);

	Tags.AddTag(
		Tags.State_ParryWindowActive,
		FName(TEXT("State.ParryWindowActive")),
		TEXT("可弹反窗口状态 - 攻击处于可被精确格挡的时间段")
	);

	// 🔧 动作状态
	Tags.AddTag(
		Tags.State_Attacking,
		FName(TEXT("State.Attacking")),
		TEXT("正在攻击状态 - 角色执行攻击动作期间")
	);
	
	Tags.AddTag(
		Tags.State_Dodging,
		FName(TEXT("State.Dodging")),
		TEXT("正在闪避状态 - 角色执行闪避动作期间")
	);
	
	Tags.AddTag(
		Tags.State_Sprint,
		FName(TEXT("State.Sprint")),
		TEXT("正在冲刺状态 - 角色加快移动速度，禁止施放技能")
	);

	// 🔧 生命周期状态
	Tags.AddTag(
		Tags.State_Dead,
		FName(TEXT("State.Dead")),
		TEXT("死亡状态 - 角色已死亡，进入死亡流程")
	);

	// 🔧 战斗状态
	Tags.AddTag(
		Tags.State_InCombat,
		FName(TEXT("State.InCombat")),
		TEXT("战斗中状态 - 影响耐力恢复速率和自动治疗")
	);
	
	Tags.AddTag(
		Tags.State_Executability,
		FName(TEXT("State.Executability")),
		TEXT("可执行处决状态 - 敌人已受到足够伤害或处于特殊状态")
	);

	// 🔧 负重状态
	Tags.AddTag(
		Tags.State_Movement_Weight_Light,
		FName(TEXT("State.Movement.Weight.Light")),
		TEXT("轻负重状态（装备 < 30%）- 快速翻滚，移动速度 110%")
	);
	
	Tags.AddTag(
		Tags.State_Movement_Weight_Medium,
		FName(TEXT("State.Movement.Weight.Medium")),
		TEXT("中负重状态（装备 30-60%）- 标准翻滚，移动速度 100%")
	);
	
	Tags.AddTag(
		Tags.State_Movement_Weight_Heavy,
		FName(TEXT("State.Movement.Weight.Heavy")),
		TEXT("重负重状态（装备 60-100%）- 缓慢翻滚，移动速度 80%，耐力消耗 +30%")
	);
	
	Tags.AddTag(
		Tags.State_Movement_Weight_Over,
		FName(TEXT("State.Movement.Weight.Over")),
		TEXT("超重状态（装备 > 100%）- 无法翻滚，移动速度 50%")
	);

	// ===== 伤害类型 Tags 初始化 =====
	
	// 🔧 基础伤害类型
	Tags.AddTag(
		Tags.Damage_Type_Physical,
		FName(TEXT("Damage.Type.Physical")),
		TEXT("物理伤害 - 来自武器或物理接触")
	);
	
	Tags.AddTag(
		Tags.Damage_Type_Magical,
		FName(TEXT("Damage.Type.Magical")),
		TEXT("魔法伤害 - 来自法术或魔法攻击")
	);

	// 🔧 元素伤害类型
	Tags.AddTag(
		Tags.Damage_Type_Fire,
		FName(TEXT("Damage.Type.Fire")),
		TEXT("火焰伤害 - 触发燃烧 DOT 效果，初始伤害 50%，持续 5 秒")
	);
	
	Tags.AddTag(
		Tags.Damage_Type_Ice,
		FName(TEXT("Damage.Type.Ice")),
		TEXT("冰冻伤害 - 触发冻结减速效果，减速 50%，持续 3 秒")
	);

	// ===== 受击反应 Tags 初始化 =====
	
	// 🔧 受击强度等级
	Tags.AddTag(
		Tags.HitReact_Light,
		FName(TEXT("HitReact.Light")),
		TEXT("轻微受击 - 伤害 < MaxHealth * 10%，动画幅度小")
	);
	
	Tags.AddTag(
		Tags.HitReact_Medium,
		FName(TEXT("HitReact.Medium")),
		TEXT("中等受击 - 伤害 10-30% MaxHealth，明显后退")
	);
	
	Tags.AddTag(
		Tags.HitReact_Heavy,
		FName(TEXT("HitReact.Heavy")),
		TEXT("重度受击 - 伤害 >= 30% MaxHealth，长时间硬直")
	);
	
	Tags.AddTag(
		Tags.HitReact_Knockback,
		FName(TEXT("HitReact.Knockback")),
		TEXT("击飞反应 - 角色被击飞起来，在空中无法控制")
	);
	
	Tags.AddTag(
		Tags.HitReact_Knockdown,
		FName(TEXT("HitReact.Knockdown")),
		TEXT("倒地反应 - 角色被击倒在地，长时间硬直")
	);

	// ===== GameplayEffect 相关 Tags 初始化 =====
	
	// 🔧 持续伤害效果（DOT）
	Tags.AddTag(
		Tags.Effect_DOT,
		FName(TEXT("Effect.DOT")),
		TEXT("持续伤害效果父标签 - 所有 DOT 效果都应包含此标签")
	);
	
	Tags.AddTag(
		Tags.Effect_DOT_Fire_Burn,
		FName(TEXT("Effect.DOT.Fire.Burn")),
		TEXT("燃烧效果 - 火焰伤害引发，每 0.5 秒造成基础伤害 15%，最多 3 层堆叠")
	);
	
	Tags.AddTag(
		Tags.Effect_DOT_Ice_Frozen,
		FName(TEXT("Effect.DOT.Ice.Frozen")),
		TEXT("冻结减速效果 - 冰冻伤害引发，减速 50%，最多 3 层完全冻结")
	);

	Tags.AddTag(
		Tags.Effect_DOT_Physical_Bloodshed,
		FName(TEXT("Effect.DOT.Physical.Bloodshed")),
		TEXT("流血效果 - 物理伤害引发，每 1 秒造成基础伤害 8%，最多 5 层堆叠")
	);

	Tags.AddTag(
		Tags.Effect_DOT_Magic_Knockback,
		FName(TEXT("Effect.DOT.Magic.Knockback")),
		TEXT("魔法击退效果 - 特定魔法技能引发，击退 200-300 cm")
	);

	// 🔧 持续伤害数据标签
	Tags.AddTag(
		Tags.Effect_Data,
		FName(TEXT("Effect.Data")),
		TEXT("效果数据父标签 - 所有 DOT 参数标签的父分类")
	);
	
	Tags.AddTag(
		Tags.Effect_Data_Chance,
		FName(TEXT("Effect.Data.Chance")),
		TEXT("效果应用概率标签 - 用于 UI 显示触发概率")
	);
	
	Tags.AddTag(
		Tags.Debuff_Data_Damage,
		FName(TEXT("Debuff.Data.Damage")),
		TEXT("Debuff 伤害标签 - 标记 DOT 效果的每次伤害值")
	);
	
	Tags.AddTag(
		Tags.Debuff_Data_Frequency,
		FName(TEXT("Debuff.Data.Frequency")),
		TEXT("Debuff 触发频率标签 - 标记 DOT 每多久触发一次")
	);
	
	Tags.AddTag(
		Tags.Debuff_Data_Duration,
		FName(TEXT("Debuff.Data.Duration")),
		TEXT("Debuff 持续时长标签 - 标记 DOT 的总持续时间")
	);

	// ===== 装备槽位 Tags 初始化 =====
	
	// 🔧 武器槽位
	Tags.AddTag(
		Tags.Equipment_Slot_MainHand,
		FName(TEXT("Equipment.Slot.MainHand")),
		TEXT("主手武器槽 - 角色右手武器（右利手）")
	);
	
	Tags.AddTag(
		Tags.Equipment_Slot_OffHand,
		FName(TEXT("Equipment.Slot.OffHand")),
		TEXT("副手槽 - 角色左手槽位，可装备盾牌或双持武器")
	);

	// 🔧 防具槽位
	Tags.AddTag(
		Tags.Equipment_Slot_Head,
		FName(TEXT("Equipment.Slot.Head")),
		TEXT("头盔槽 - 头部防具，影响头部碰撞伤害")
	);
	
	Tags.AddTag(
		Tags.Equipment_Slot_Chest,
		FName(TEXT("Equipment.Slot.Chest")),
		TEXT("胸甲槽 - 躯干防具，提供最多伤害减免")
	);
	
	Tags.AddTag(
		Tags.Equipment_Slot_Gloves,
		FName(TEXT("Equipment.Slot.Gloves")),
		TEXT("手套槽 - 手部防具，微幅影响攻击速度")
	);
	
	Tags.AddTag(
		Tags.Equipment_Slot_Legs,
		FName(TEXT("Equipment.Slot.Legs")),
		TEXT("腿甲槽 - 腿部防具，影响移动速度")
	);

	// ===== 武器类型 Tags 初始化 =====
	
	// 🔧 武器类型分类
	Tags.AddTag(
		Tags.Weapon_Type_Sword,
		FName(TEXT("Weapon.Type.Sword")),
		TEXT("单手剑 - 轻型剑类武器，平衡攻防，可双持")
	);
	
	Tags.AddTag(
		Tags.Weapon_Type_Greatsword,
		FName(TEXT("Weapon.Type.Greatsword")),
		TEXT("巨剑/大剑 - 重型剑类武器，高伤害但速度慢，不可双持")
	);
	
	Tags.AddTag(
		Tags.Weapon_Type_Staff,
		FName(TEXT("Weapon.Type.Staff")),
		TEXT("法杖 - 魔法类武器，不进行物理近战攻击，触发法术技能")
	);

	// 📝 初始化完成日志
	UE_LOG(
		LogTemp, 
		Warning, 
		TEXT("✓ GameplayTags 初始化完成 - 已注册 %d 个标签，所有 Tag 已准备就绪！"),
		105  // 🔧 修改 - 根据实际 Tag 总数更新此数字（目前统计为 105 个）
	);
}


/**
 * @brief 注册并获取一个原生的 Gameplay Tag (游戏性标签)
 * 这是一个静态辅助函数，用于减少重复代码。它调用底层的 Tag 管理器来注册标签，并将结果赋值给传入的变量。
 * * @param OutTag      [输出] 引用参数。注册成功后，对应的 FGameplayTag 结构体会存储在这个变量中。
 * @param TagName     [输入] 标签的名称（例如 "Attributes.Secondary.Armor"）。必须是唯一的 FName。
 * @param TagComment  [输入] 标签的描述/注释。这会在虚幻编辑器的 Project Settings -> Gameplay Tags 界面中显示，用于提示开发者该标签的作用。
 */
void FZBGameplayTags::AddTag(
	FGameplayTag& OutTag,
	const FName& TagName,
	const FString& TagComment
)
{
	// 1. UGameplayTagsManager::Get()
	// 获取 GameplayTagsManager 的单例实例。这是 UE 专门管理所有 Gameplay Tag 的核心系统。
    
	// 2. AddNativeGameplayTag(...)
	// 向管理器注册一个“原生（Native）”标签。
	// "原生"意味着这个标签是在 C++ 代码中定义的，而不是仅在编辑器的 INI 文件中配置的。
	// 如果该名称的标签已经存在，管理器会返回已有的标签；如果不存在，则创建一个新的。
	// ✨ 新增 - 获取 GameplayTagsManager 单例，用于注册原生 Tag
	UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();
	
	// 🔧 修改 - 调用 RequestGameplayTag 注册 Tag 到系统
	// 该方法返回已注册或新创建的 Tag 对象
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
	  TagName,
	  TagComment
  );

	// 📝 调试日志：输出每个 Tag 的注册情况
	if (OutTag.IsValid())
	{

		UE_LOG(
			LogTemp,
			Verbose,
			TEXT("✓ GameplayTag 注册成功：%s - %s"),
			*TagName.ToString(),
			*TagComment
		);
	}
	else
	{

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("✗ GameplayTag 注册失败或无效：%s - %s，请检查 Tag 命名是否规范"),
			*TagName.ToString(),
			*TagComment
		);
	}
}
