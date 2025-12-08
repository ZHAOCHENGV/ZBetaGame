// Fill out your copyright notice in the Description page of Project Settings.
/**
 * @file ZBGameplayTags.h
 * @brief 项目全局 GameplayTags 定义
 * 
 * 功能说明：
 *   集中定义和管理项目中所有的 GameplayTags。
 *   使用原生 Tag 而非字符串，确保编译期类型安全。
 *   采用单例模式，全局访问。
 * 
 * 详细流程：
 *   1. 在模块启动时调用 InitializeNativeTags() 注册所有 Tag
 *   2. 运行时通过 FZBGameplayTags::Get() 访问 Tag 实例
 *   3. 在 Ability、GE、代码中直接使用这些 Tag
 * 
 * 注意事项：
 *   - 所有 Tag 必须在 InitializeNativeTags 中注册
 *   - Tag 命名采用层级结构（如 State.IFrame）
 *   - 新增 Tag 时同时更新此文件和 DefaultGameplayTags.ini
 */

/**
 * @file    ZBGameplayTags.h
 * @brief   项目全局 GameplayTags 定义与管理
 * @author  Unreal Engine 5.7 | GAS Framework
 * 
 * @详细说明
 *   该模块集中定义和管理项目中所有的 GameplayTags，采用原生 FGameplayTag 而非字符串，
 *   确保编译期类型安全和运行时高效查询。整个系统采用单例模式设计，通过 FZBGameplayTags::Get()
 *   实现全局唯一访问点。
 * 
 * @工作流程
 *   1. 项目启动：GameModule 的 StartupModule() 调用 InitializeNativeTags()
 *   2. Tag 注册：所有 Tag 通过 AddTag() 向 GameplayTagManager 注册
 *   3. 运行时访问：全局任意位置通过 FZBGameplayTags::Get().TagName 获取 Tag
 *   4. 能力使用：在 GA、GE、ASC 中直接使用这些 Tag 控制逻辑
 * 
 * @性能架构注意事项
 *   - Tag 是预编译常量，零运行时字符串查询开销
 *   - FGameplayTagContainer 底层使用 TBitArray，容器操作 O(1) 高效
 *   - 避免在 Tick() 中创建临时 Tag，改用此单例中预定义的 Tag
 *   - Tag 命名必须遵循层级结构（最深3层为佳），便于编辑器匹配和代码维护
 * 
 * @使用规范示例
 *   // 在 GA 中检查状态
 *   if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(FZBGameplayTags::Get().State_IFrame))
 *   {
 *       return; // 无敌帧中，无法施放
 *   }
 *   
 *   // 在 GE 中添加标签
 *   OutSpec.DynamicGrantedTags.AddTag(FZBGameplayTags::Get().State_Attacking);
 *   
 *   @新增Tag流程
 *   1. 在此文件中添加 FGameplayTag 成员变量
 *   2. 在 .cpp 中的 InitializeNativeTags() 调用 AddTag()
 *   3. 同步更新 DefaultGameplayTags.ini（若使用数据表配置）
 */


#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


/**
 * @class   FZBGameplayTags
 * @brief   项目全局 GameplayTags 单例容器
 * 
 * @设计意图
 *   - 提供编译期安全的 Tag 访问，避免硬编码字符串
 *   - 集中管理 Tag 生命周期，确保所有 Tag 在使用前已初始化
 *   - 按功能分类组织，便于维护和扩展
 * 
 * @线程安全
 *   - 初始化阶段（启动时）：单线程执行
 *   - 运行时：所有 Tag 为 const 只读，线程安全
 * 
 * @内存占用
 *   - 单例大小：~4-8 KB（纯指针/枚举值）
 *   - 不存储实际字符串，字符串由 UGameplayTagsManager 统一管理
 */
struct FZBGameplayTags
{
 public:
	/**
	 * @brief   获取全局 GameplayTags 单例实例
	 * @return  const FZBGameplayTags& - 唯一的全局 Tags 容器引用
	 * @note    该方法为纯 const，可安全地在任意上下文调用
	 * 
	 * @使用示例
	 *   const FGameplayTag& HealthTag = FZBGameplayTags::Get().Attributes_Vital_Health;
	 */
	static const FZBGameplayTags& Get() { return GameplayTags; }

	/**
	 * @brief   初始化所有原生 GameplayTags
	 * 
	 * @工作流程
	 *   1. 遍历所有 Tag 成员变量
	 *   2. 对每个 Tag 调用 AddTag() 进行注册
	 *   3. 向 UGameplayTagsManager 提交标准化后的 Tag
	 *   4. 验证所有 Tag 的有效性
	 * 
	 * @调用时机
	 *   - 模块启动阶段：FZBGameModule::StartupModule()
	 *   - 必须在任何 GameplayAbilitySystem 初始化前完成
	 * 
	 * @后置条件
	 *   - 所有 Tag 成员变量已初始化并有效
	 *   - 可安全地使用 Get() 获取 Tags
	 * 
	 * @调用约束
	 *   - 仅调用一次（重复调用会导致标签重复注册警告）
	 *   - 必须在主线程执行
	 * 
	 * @性能开销
	 *   - 首次调用：O(n)，n 为 Tag 总数（仅发生一次）
	 *   - 对帧率无影响（启动时执行）
	 */
	static void InitializeNativeTags();

	// ========================================
	// 第一部分：属性相关 Tags（Attributes）
	// ========================================
	// 用途：标识角色属性类型，关联 AttributeSet 中的 FGameplayAttributeData
	// 规范：所有属性 Tag 以 Attributes_ 前缀开头，按属性大类分组
	
	/**
	 * @section 生命值系列（Vital - 生命值相关）
	 * @brief   标识角色核心生命值属性
	 */
	
	/** 当前生命值 - 范围 [0, MaxHealth] */
	FGameplayTag Attributes_Vital_Health;
	
	/** 当前法力值 - 范围 [0, MaxMana]，用于法术消耗 */
	FGameplayTag Attributes_Vital_Mana;
	
	/** 当前耐力值 - 范围 [0, MaxStamina]，用于翻滚/冲刺消耗 */
	FGameplayTag Attributes_Vital_Stamina;
	
	/** 当前韧性值 - 范围 [0, MaxToughness]，防止被打断 */
	FGameplayTag Attributes_Vital_Toughness;
	
	/** 力量属性 - 影响物理伤害倍数 */
	FGameplayTag Attributes_Vital_Strength;
	
	/** 智力属性 - 影响魔法伤害倍数 */
	FGameplayTag Attributes_Vital_Intelligence;
	
	/** 敏捷属性 - 影响移动速度和闪避 */
	FGameplayTag Attributes_Vital_Dexterity;

	/**
	 * @section 最大值系列（Max - 属性上限）
	 * @brief   标识属性的最大值，决定恢复和增强效果的上限
	 */
	
	/** 最大生命值上限 */
	FGameplayTag Attributes_Max_MaxHealth;
	
	/** 最大法力值上限 */
	FGameplayTag Attributes_Max_MaxMana;
	
	/** 最大耐力值上限 */
	FGameplayTag Attributes_Max_MaxStamina;
	
	/** 最大韧性值上限 */
	FGameplayTag Attributes_Max_Toughness;

	/**
	 * @section 抗性系列（Resistance - 伤害抗性）
	 * @brief   标识不同伤害类型的抗性修饰符
	 */
	
	/** 物理伤害抗性 - 百分比值 [0~100] */
	FGameplayTag Attributes_Resistance_Physical;
	
	/** 魔法伤害抗性 - 百分比值 [0~100] */
	FGameplayTag Attributes_Resistance_Magical;

	/**
	 * @section 恢复速率系列（RegenRate - 自然恢复）
	 * @brief   标识各属性的自然恢复速率（每秒）
	 * @note    仅在非战斗状态下触发，战斗中需要特殊 GE
	 */
	
	/** 生命值恢复速率 - 单位：点/秒 */
	FGameplayTag Attributes_RegenRate_Health;
	
	/** 法力值恢复速率 - 单位：点/秒 */
	FGameplayTag Attributes_RegenRate_Mana;
	
	/** 耐力值恢复速率 - 单位：点/秒，战斗中触发较快 */
	FGameplayTag Attributes_RegenRate_Stamina;
	
	/** 韧性值恢复速率 - 单位：点/秒 */
	FGameplayTag Attributes_RegenRate_Toughness;

	/**
	 * @section 耐力成本倍数系列（StaminaCostMultiplier）
	 * @brief   标识不同动作的耐力消耗倍数修饰符
	 * @note    基础消耗 × 倍数 = 最终消耗，用于难度调整或 Buff 效果
	 */
	
	/** 闪避耐力消耗倍数 - 默认1.0，> 1 表示消耗增加 */
	FGameplayTag Attributes_StaminaCostMultiplier_Dodge;
	
	/** 冲刺耐力消耗倍数 - 默认1.0 */
	FGameplayTag Attributes_StaminaCostMultiplier_Sprint;

	/**
	 * @section 吸取系列（Steal）
	 * @brief   标识攻击命中时的属性吸取效果（如吸血）
	 * @note    用于特殊武器或技能的生命窃取机制
	 */
	
	/** 生命吸取百分比 - [0~100] */
	FGameplayTag Attributes_Steal_Health;
	
	/** 法力吸取百分比 - [0~100] */
	FGameplayTag Attributes_Steal_Mana;
	
	/** 耐力吸取百分比 - [0~100] */
	FGameplayTag Attributes_Steal_Stamina;

	/**
	 * @section 暴击系列（Critical）
	 * @brief   标识暴击相关的属性
	 */
	
	/** 暴击率 - 百分比 [0~100] */
	FGameplayTag Attributes_Critical_Chance;
	
	/** 暴击伤害倍数 - 1.0 = 100% 额外伤害 */
	FGameplayTag Attributes_Critical_Damage;

	/**
	 * @section 统计数据系列（Stats - 其他通用属性）
	 * @brief   标识游戏规则相关的属性
	 */
	
	/** 移动速度 - 单位：cm/s */
	FGameplayTag Attributes_Stats_MoveSpeed;
	
	/** 最大装备负载 - 单位：kg，超负载影响闪避 */
	FGameplayTag Attributes_Stats_MaxEquipmentLoad;

	/**
	 * @section 元数据系列（Meta - 系统属性）
	 * @brief   标识游戏系统级属性（如经验、货币等）
	 * @note    通常用于 GE 中临时存储，不作为 ASC 永久属性
	 */
	
	/** 接收的经验值 - 临时存储，用于 ModifyAttribute 回调触发 */
	FGameplayTag Attributes_Meta_IncomingXP;

	
	// ========================================
	// 第二部分：输入相关 Tags（Input）
	// ========================================
	// 用途：与 Enhanced Input System 结合，标识玩家输入事件
	// 规范：所有输入 Tag 以 InputTag_ 前缀开头
	// 工作流程：Input → InputTag → GA 激活条件
	
	/**
	 * @section 攻击输入
	 * @brief   标识攻击类输入事件
	 */
	
	/** 
	 * 普通攻击输入（主键位）
	 * 工作机制：
	 *   - 短按（< 0.3s）：触发轻攻击 GA
	 *   - 长按（> 0.3s）：触发重攻击 GA
	 * 组合：可与特定方向键组合形成衍生攻击
	 */
	FGameplayTag InputTag_Attack_Main;

	/**
	 * @section 防守/机动输入
	 * @brief   标识防守和机动相关的输入
	 */
	
	/** 
	 * 闪避输入
	 * 规则：
	 *   - 需要消耗耐力
	 *   - 闪避成功时获得无敌帧
	 *   - 可与方向键组合（前后左右闪避）
	 */
	FGameplayTag InputTag_Dodge;
    
	/** 
	 * 格挡输入
	 * 规则：
	 *   - 按住时持续格挡（消耗耐力）
	 *   - 格挡可减少伤害并积累破防值
	 *   - 破防时触发 State_GuardBroken
	 */
	FGameplayTag InputTag_Block;

	/** 
	 * 冲刺输入
	 * 规则：
	 *   - 持续消耗耐力
	 *   - 触发 State_Sprint，加快移动速度
	 *   - 在冲刺状态下无法施放技能
	 */
	FGameplayTag InputTag_Sprint;

	/**
	 * @section 技能输入
	 * @brief   标识符文技能（特殊能力）的输入槽位
	 * @note    符文技能系统支持 4 个快捷槽位
	 */
	
	/** 符文技能槽位1输入 */
	FGameplayTag InputTag_Rune_1;
    
	/** 符文技能槽位2输入 */
	FGameplayTag InputTag_Rune_2;
    
	/** 符文技能槽位3输入 */
	FGameplayTag InputTag_Rune_3;
    
	/** 符文技能槽位4输入 */
	FGameplayTag InputTag_Rune_4;

	/**
	 * @section 交互/UI 输入
	 * @brief   标识非战斗相关的系统输入
	 */
	
	/** 
	 * 交互输入
	 * 用途：靠近 NPC、宝箱、传送点时激活
	 */
	FGameplayTag InputTag_Interaction;
    
	/** 
	 * 锁定目标输入
	 * 功能：
	 *   - 激活/关闭锁定目标模式
	 *   - 影响相机跟踪和攻击方向
	 */
	FGameplayTag InputTag_TargetLock;

	/** 
	 * 菜单输入
	 * 用途：打开/关闭主菜单、背包等 UI
	 */
	FGameplayTag InputTag_Menus;

	/** 
	 * 药水/消耗品输入
	 * 用途：快速使用背包中的消耗品
	 */
	FGameplayTag InputTag_Consumable;

	
	// ========================================
	// 第三部分：能力标识 Tags（Ability）
	// ========================================
	// 用途：标识能力的类型和归类，便于查询和管理
	// 规范：所有能力 Tag 以 Ability_ 前缀开头
	// 关键特性：用于 GA::BlockAbilitiesWithTag/CancelAbilitiesWithTag 等操作
	
	/**
	 * @section 攻击能力
	 * @brief   标识各种攻击类能力
	 */
	
	/** 
	 * 轻攻击能力标签
	 * 含义：标记所有轻攻击类 GA，便于集中取消或阻止
	 * 应用：
	 *   - 在某些特殊状态下，阻止轻攻击但允许重攻击
	 *   - 追踪轻攻击命中次数（连击计数）
	 */
	FGameplayTag Ability_Attack_Light;
    
	/** 
	 * 重攻击能力标签
	 * 含义：标记所有重攻击类 GA，通常消耗更多资源但伤害更高
	 */
	FGameplayTag Ability_Attack_Heavy;

	/**
	 * @section 防守/机动能力
	 * @brief   标识防守和闪避类能力
	 */
	
	/** 
	 * 闪避能力标签
	 * 含义：标记所有闪避动作
	 * 应用：在某些 GE 中禁用闪避（如减速），设置 BlockAbilitiesWithTag
	 */
	FGameplayTag Ability_Dodge;
    
	/** 
	 * 格挡能力标签
	 * 含义：标记所有格挡相关的 GA
	 */
	FGameplayTag Ability_Block;
    
	/** 
	 * 招架能力标签（精确格挡）
	 * 含义：标记精确格挡/弹反窗口，成功后无伤并反击
	 * 区别：招架是极短时间窗口的格挡，需要精确时机
	 */
	FGameplayTag Ability_Parry;

	/**
	 * @section 技能能力
	 * @brief   标识技能相关的能力
	 */
	
	/** 
	 * 符文技能父标签
	 * 用途：所有符文技能都应带有此标签，便于整体管理
	 * 应用：某些 Buff 可禁用所有符文技能，使用 BlockAbilitiesWithTag(Ability_Rune)
	 */
	FGameplayTag Ability_Rune;

	
	// ========================================
	// 第四部分：角色状态 Tags（State）
	// ========================================
	// 用途：标记角色当前所处的状态，控制能力的激活条件和执行逻辑
	// 规范：所有状态 Tag 以 State_ 前缀开头
	// 关键原理：通过 ASC 的 AddLooseGameplayTag/RemoveLooseGameplayTag 实时管理
	// 性能优化：状态 Tag 采用 bitflag 存储，O(1) 查询时间
	
	/**
	 * @section 帧状态
	 * @brief   标识角色当前所处的特殊帧状态
	 */
	
	/** 
	 * 无敌帧状态
	 * 含义：角色处于无敌帧中，所有伤害免疫
	 * 触发时机：
	 *   - 闪避动作执行期间
	 *   - 招架成功时
	 *   - 某些技能的无敌帧段
	 * 应用：GE 的 ConditionalGameplayEffects 中检查此标签，决定是否应用伤害
	 * 建议：与 Animation Notify 结合，在蒙太奇帧标记中精确控制
	 */
	FGameplayTag State_IFrame;
    
	/** 
	 * 霸体状态（超甲）
	 * 含义：角色处于霸体状态，不会被打断或击飞
	 * 触发时机：
	 *   - 某些重击攻击的投入阶段
	 *   - 大招施放期间
	 *   - 特定 BUFF 效果
	 * 应用：GE 中的 GameplayEffectTags 包含此标签时检查
	 * 注意：霸体 ≠ 无敌，仍然受到伤害，但不被打断
	 */
	FGameplayTag State_HyperArmor;
    
	/** 
	 * 格挡中状态
	 * 含义：角色正在主动格挡，处于防守动作中
	 * 触发条件：InputTag_Block 被激活且有足够耐力
	 * 应用：
	 *   - 控制格挡中的伤害减免
	 *   - 在格挡时禁止某些移动
	 * 建议：配合动画混合空间，将格挡姿态渐变到移动
	 */
	FGameplayTag State_Blocking;
    
	/** 
	 * 硬直中状态（被打断）
	 * 含义：角色受到打击后进入硬直状态，无法执行任何操作
	 * 触发条件：
	 *   - 受到超过韧性值的伤害
	 *   - 被击飞/倒地后恢复
	 * 持续时间：根据伤害大小动态调整（通过 GE Duration）
	 * 应用：GE 添加此标签，同时阻止其他 GA 激活
	 */
	FGameplayTag State_Staggered;
    
	/** 
	 * 破防状态
	 * 含义：角色格挡值达到上限，防守能力下降
	 * 触发条件：格挡累计伤害 > GuardBreakThreshold
	 * 效果：
	 *   - 格挡伤害减免降低 50%
	 *   - 格挡耐力消耗翻倍
	 *   - 持续 2-3 秒后自动恢复
	 * 应用：配合独特的击飞动画和视觉效果
	 */
	FGameplayTag State_GuardBroken;
    
	/** 
	 * 可取消窗口状态
	 * 含义：当前动作可被后续输入打断或取消
	 * 触发时机：
	 *   - 动作执行到特定帧（通过 AnimNotify 标记）
	 *   - 某些连击窗口打开时
	 * 应用：GA 在执行时检查此标签，决定是否允许新的输入立即响应
	 * 性能优化：避免频繁查询，改用 AnimNotify 触发事件回调
	 */
	FGameplayTag State_CanCancel;
    
	/** 
	 * 命中窗口激活状态
	 * 含义：攻击动作的有效伤害帧已打开，命中检测激活
	 * 触发时机：
	 *   - AnimNotify_BeginHitWindow 触发时
	 *   - 直到 AnimNotify_EndHitWindow 关闭
	 * 应用：
	 *   - 系统在此窗口期间执行 TraceLineForObjects 检测
	 *   - 防止多次命中同一目标
	 *   - 可视化调试：在编辑器中显示命中检测范围
	 */
	FGameplayTag State_HitWindowActive;

	/** 
	 * 可弹反窗口状态
	 * 含义：攻击处于可被精确格挡/弹反的时间段
	 * 触发时机：
	 *   - 比 HitWindow 更短，通常是攻击高潮时刻前 0.2-0.3 秒
	 * 应用：
	 *   - Parry GA 检查此标签，判断是否成功弹反
	 *   - 成功弹反后，攻击者进入硬直状态
	 * 建议：与 Animation Montage 的 Notify 轨迹精确同步
	 */
	FGameplayTag State_ParryWindowActive;

	/**
	 * @section 动作状态
	 * @brief   标识角色正在执行的动作
	 */
	
	/** 
	 * 正在攻击状态
	 * 含义：角色执行攻击动作期间的总体状态
	 * 触发条件：任何攻击 GA 激活时添加此标签
	 * 应用：
	 *   - 禁止重复输入相同的攻击
	 *   - 影响移动速度（攻击时可能减速）
	 *   - 用于 Montage 同步检查
	 */
	FGameplayTag State_Attacking;
    
	/** 
	 * 正在闪避状态
	 * 含义：角色执行闪避动作期间
	 * 触发条件：Dodge GA 激活时添加此标签
	 * 持续时间：闪避蒙太奇完成后移除
	 * 应用：闪避中禁止再次闪避（直到 i-frame 结束后）
	 */
	FGameplayTag State_Dodging;

	/** 
	 * 正在冲刺状态
	 * 含义：角色处于冲刺动作中，加快移动速度
	 * 持续时间：InputTag_Sprint 持续输入期间
	 * 应用：
	 *   - 禁止施放技能
	 *   - 提高移动速度 150%
	 *   - 持续消耗耐力
	 */
	FGameplayTag State_Sprint;

	/**
	 * @section 生命周期状态
	 * @brief   标识角色的生命状态
	 */
	
	/** 
	 * 死亡状态
	 * 含义：角色已死亡，进入死亡流程
	 * 触发条件：Health 属性 <= 0
	 * 后续流程：
	 *   1. 移除所有可取消的 GA
	 *   2. 播放死亡蒙太奇
	 *   3. 禁用碰撞
	 *   4. 触发死亡回调和掉落物品逻辑
	 *   5. 在一定延迟后销毁或回收
	 */
	FGameplayTag State_Dead;

	/**
	 * @section 战斗状态
	 * @brief   标识角色的整体战斗状态
	 */
	
	/** 
	 * 战斗中状态
	 * 含义：角色处于活跃战斗中，影响各种行为
	 * 触发条件：
	 *   - 首次发起或受到伤害
	 *   - 主动进入战斗范围
	 * 应用：
	 *   - 战斗中：耐力恢复速率提升 2 倍
	 *   - 战斗中：禁止自动治疗
	 *   - UI 显示战斗指示器
	 *   - 离开战斗后 10 秒自动解除标签
	 */
	FGameplayTag State_InCombat;

	/** 
	 * 可执行处决状态
	 * 含义：当前敌人已受到足够伤害或处于特殊状态，可执行处决技能
	 * 触发条件：
	 *   - 敌人 Health < MaxHealth * 20%
	 *   - 或处于特定状态（倒地、硬直等）
	 * 应用：
	 *   - 显示屏幕提示（"按 E 处决"）
	 *   - Finisher GA 的激活条件检查
	 */
	FGameplayTag State_Executability;

	/**
	 * @section 负重状态
	 * @brief   标识角色的装备负重等级，影响闪避动画和速度
	 * @详细说明
	 *   根据当前装备总重量与最大承载的比值，动态切换角色的负重等级。
	 *   不同负重等级有不同的闪避动作（快滚/标准滚/扑倒）和移动速度系数。
	 */
	
	/** 
	 * 轻负重状态（装备负载 < 30%）
	 * 特性：
	 *   - 闪避动画：快速翻滚（旋转角度小）
	 *   - 移动速度系数：110%（基础速度）
	 *   - 闪避耐力消耗：-20% 优惠
	 */
	FGameplayTag State_Movement_Weight_Light;
	
	/** 
	 * 中负重状态（装备负载 30%-60%）
	 * 特性：
	 *   - 闪避动画：标准翻滚
	 *   - 移动速度系数：100%（基础速度）
	 *   - 闪避耐力消耗：标准值
	 */
	FGameplayTag State_Movement_Weight_Medium;
	
	/** 
	 * 重负重状态（装备负载 60%-100%）
	 * 特性：
	 *   - 闪避动画：缓慢翻滚/向侧扑倒
	 *   - 移动速度系数：80%（基础速度）
	 *   - 闪避耐力消耗：+30% 增加
	 */
	FGameplayTag State_Movement_Weight_Heavy;
	
	/** 
	 * 超重状态（装备负载 > 100%）
	 * 特性：
	 *   - 闪避动画：不可闪避，进入"超重"状态
	 *   - 移动速度系数：50%（严重减速）
	 *   - 能力：Dodge GA 的 CanActivate 检查此标签，返回 false
	 *   - 视觉效果：角色动画速度变慢，步伐沉重
	 * 警告：超重状态下高度脆弱，需要移除装备或分散负载
	 */
	FGameplayTag State_Movement_Weight_Over;

	
	// ========================================
	// 第五部分：伤害类型 Tags（Damage Type）
	// ========================================
	// 用途：标识伤害的元素/物理属性，决定抗性计算和触发效果
	// 规范：所有伤害类型 Tag 以 Damage_Type_ 前缀开头
	// 工作流程：GA 计算伤害 → 标记伤害类型 → 触发相应 DOT/效果
	
	/**
	 * @section 基础伤害类型
	 * @brief   标识伤害的基本属性
	 */
	
	/** 
	 * 物理伤害
	 * 含义：来自武器、拳击等物理接触的伤害
	 * 抗性对应：Attributes_Resistance_Physical
	 * 特殊效果：可能触发 Effect_DOT_Physical_Bloodshed（流血）
	 */
	FGameplayTag Damage_Type_Physical;

	/** 
	 * 魔法伤害
	 * 含义：来自法术、魔法攻击的伤害
	 * 抗性对应：Attributes_Resistance_Magical
	 * 特殊效果：可能触发各种元素 DOT 效果
	 */
	FGameplayTag Damage_Type_Magical;

	/**
	 * @section 元素伤害类型
	 * @brief   标识具体元素属性的伤害
	 */
	
	/** 
	 * 火焰伤害
	 * 含义：具有火焰属性的伤害
	 * 默认 DOT 效果：Effect_DOT_Fire_Burn（燃烧，持续伤害）
	 * DOT 参数：
	 *   - 初始伤害：原伤害的 50%
	 *   - 持续时间：5 秒
	 *   - 触发频率：每 0.5 秒一次
	 */
	FGameplayTag Damage_Type_Fire;
    
	/** 
	 * 冰冻伤害
	 * 含义：具有冰冻属性的伤害
	 * 默认 DOT 效果：Effect_DOT_Ice_Frozen（冻结减速）
	 * DOT 参数：
	 *   - 移动速度减速：50%
	 *   - 持续时间：3 秒
	 *   - 多次冰冻可堆叠，延长持续时间或叠加减速
	 * 特殊：完全冻结（3 层堆叠）后，目标无法移动
	 */
	FGameplayTag Damage_Type_Ice;


	// ========================================
	// 第六部分：受击反应 Tags（HitReact）
	// ========================================
	// 用途：标识受击时的反应强度和动画类型
	// 规范：所有受击反应 Tag 以 HitReact_ 前缀开头
	// 原理：GA 根据伤害值选择合适的 HitReact Tag，触发对应蒙太奇
	
	/**
	 * @section 受击强度等级
	 * @brief   标识伤害的强度等级，对应不同的受击动画
	 * @工作流程
	 *   1. GA 计算伤害值
	 *   2. 根据伤害与角色 MaxHealth 的比值选择反应等级
	 *   3. 播放对应等级的受击蒙太奇（Light/Medium/Heavy）
	 *   4. 添加相应的 HitReact Tag（用于视觉反馈和后续逻辑）
	 */
	
	/** 
	 * 轻微受击
	 * 含义：受到较小伤害，受击动画幅度小
	 * 触发条件：伤害值 < MaxHealth * 10%
	 * 特性：
	 *   - 动画：轻微摇晃，无位移
	 *   - 硬直时间：0.3 秒
	 *   - 动作不被打断，可继续攻击
	 * 应用：打击感反馈，但不影响战斗流畅性
	 */
	FGameplayTag HitReact_Light;
    
	/** 
	 * 中等受击
	 * 含义：受到中等伤害，动画明显
	 * 触发条件：MaxHealth * 10% <= 伤害值 < MaxHealth * 30%
	 * 特性：
	 *   - 动画：明显后退，角色被击退 100-200 cm
	 *   - 硬直时间：0.5 秒
	 *   - 正在执行的动作被打断（进入 Staggered 状态）
	 * 应用：敌人受到明显影响，玩家能感受到伤害权重
	 */
	FGameplayTag HitReact_Medium;
    
	/** 
	 * 重度受击
	 * 含义：受到重击，角色失去平衡
	 * 触发条件：伤害值 >= MaxHealth * 30%
	 * 特性：
	 *   - 动画：大幅后退或倒地
	 *   - 硬直时间：1.5-2.0 秒
	 *   - 长时间无法行动
	 *   - 可能触发额外击飞效果
	 */
	FGameplayTag HitReact_Heavy;
    
	/** 
	 * 击飞反应
	 * 含义：角色被击飞起来，进入空中状态
	 * 触发条件：
	 *   - 特定攻击的击飞效果
	 *   - 重击 + 敌人韧性不足
	 * 特性：
	 *   - 角色在空中无法控制
	 *   - 落地后播放起身动画
	 *   - 可能造成额外摔伤伤害
	 * 物理应用：使用 Character Movement 的 Velocity 实现抛物线
	 */
	FGameplayTag HitReact_Knockback;
    
	/** 
	 * 倒地反应
	 * 含义：角色被击倒在地
	 * 触发条件：
	 *   - 极重伤害
	 *   - 从高处坠落
	 *   - 特定攻击的倒地效果
	 * 特性：
	 *   - 播放倒地蒙太奇
	 *   - 进入长时间硬直状态（2-3 秒）
	 *   - 地上状态下可能被敌人执行处决
	 *   - 可主动按键快速起身（消耗大量耐力）
	 */
	FGameplayTag HitReact_Knockdown;


	// ========================================
	// 第七部分：GameplayEffect 相关 Tags
	// ========================================
	// 用途：标识 GE 的效果类型和参数
	// 规范：所有 GE 相关 Tag 以 Effect_ 或 Debuff_ 前缀开头
	// 工作流程：GA 创建 GE Spec → 设置 Tag → 应用到 Target ASC
	
	/**
	 * @section 持续伤害效果（DOT - Damage Over Time）
	 * @brief   标识各种持续伤害效果，实现中毒、燃烧等机制
	 * @实现原理
	 *   1. 攻击命中时，GA 创建 DOT GE
	 *   2. GE 中 InheritableGameplayEffectTags 包含具体 DOT 标签
	 *   3. 目标 ASC 定期（每 0.5-1 秒）应用一次伤害
	 *   4. 堆叠策略：部分 DOT 可堆叠延长，部分则刷新时间
	 */
	
	/** 
	 * 持续伤害效果父标签
	 * 含义：所有 DOT 效果都应包含此标签作为分类
	 * 应用：某些技能或 Buff 可移除所有带 Effect_DOT 的 GE
	 */
	FGameplayTag Effect_DOT;
    
	/** 
	 * 燃烧效果
	 * 含义：目标处于燃烧状态，持续受火焰伤害
	 * 触发条件：受到 Damage_Type_Fire 伤害时，50% 概率触发
	 * GE 配置：
	 *   - Duration：5 秒
	 *   - Period：0.5 秒（每半秒触发一次伤害）
	 *   - Damage Per Tick：基础伤害的 15%
	 *   - 堆叠策略：Duration Refresh（刷新时间，最多 3 层）
	 * 视觉效果：角色模型附加火焰粒子效果
	 */
	FGameplayTag Effect_DOT_Fire_Burn;
	
	/** 
	 * 冻结减速效果
	 * 含义：目标处于冻结状态，移动速度大幅降低
	 * 触发条件：受到 Damage_Type_Ice 伤害时，100% 触发
	 * GE 配置：
	 *   - Duration：3 秒
	 *   - 效果：应用 Attributes_Stats_MoveSpeed -50% Modifier
	 *   - 堆叠策略：Additive Aggregate（可堆叠 3 层，每层额外减速）
	 *   - 3 层时完全冻结，无法移动
	 * 视觉效果：屏幕蓝色滤镜，角色身上冰晶
	 */
	FGameplayTag Effect_DOT_Ice_Frozen;

	/** 
	 * 流血效果
	 * 含义：物理伤害造成的出血效果，造成缓慢持续伤害
	 * 触发条件：Damage_Type_Physical 伤害，30% 概率触发
	 * GE 配置：
	 *   - Duration：8 秒
	 *   - Period：1 秒
	 *   - Damage Per Tick：基础伤害的 8%
	 *   - 堆叠策略：Additive（可堆叠 5 层）
	 * 视觉效果：角色衣服和地面出现血迹
	 */
	FGameplayTag Effect_DOT_Physical_Bloodshed;

	/** 
	 * 魔法击退效果
	 * 含义：魔法伤害造成的击退效果，减少敌人威胁性
	 * 触发条件：特定魔法技能或 Damage_Type_Magical
	 * GE 配置：
	 *   - Instant 类型（不是 DOT）
	 *   - 应用 Impulse（冲量）到 Character Movement
	 *   - 击退距离：200-300 cm
	 */
	FGameplayTag Effect_DOT_Magic_Knockback;

	/**
	 * @section 持续伤害数据标签
	 * @brief   标识 DOT 效果中的具体参数信息
	 * @用途
	 *   在某些高级机制中（如 UI 显示 Debuff 信息），需要解析 GE 的参数。
	 *   这些标签帮助系统快速定位和提取 DOT 的关键参数。
	 */
	
	/** 
	 * 效果数据父标签
	 * 含义：所有 DOT 参数标签的父分类
	 */
	FGameplayTag Effect_Data;
	
	/** 
	 * 效果应用概率标签
	 * 含义：标记此 GE 的触发概率信息
	 * 应用场景：UI 显示 Debuff 时，显示触发概率百分比
	 */
	FGameplayTag Effect_Data_Chance;
	
	/** 
	 * Debuff 伤害标签
	 * 含义：标记此 DOT 效果的每次伤害值
	 */
	FGameplayTag Debuff_Data_Damage;
	
	/** 
	 * Debuff 触发频率标签
	 * 含义：标记此 DOT 每多久触发一次（Period）
	 */
	FGameplayTag Debuff_Data_Frequency;
	
	/** 
	 * Debuff 持续时长标签
	 * 含义：标记此 DOT 的总持续时间
	 */
	FGameplayTag Debuff_Data_Duration;

	
	// ========================================
	// 第八部分：装备槽位 Tags（Equipment Slot）
	// ========================================
	// 用途：标识装备系统的槽位，控制装备穿戴和替换
	// 规范：所有装备槽位 Tag 以 Equipment_Slot_ 前缀开头
	// 工作流程：装备系统 → 确定槽位 → 通过 Tag 验证装备有效性
	
	/**
	 * @section 武器槽位
	 * @brief   标识武器装备的槽位
	 */
	
	/** 
	 * 主手武器槽
	 * 含义：角色右手武器槽（对于右利手）
	 * 应用：
	 *   - 装备检测：装备类型为 Weapon 且 WeaponType 包含 Weapon_Type_Sword 时允许穿戴
	 *   - 攻击动作：主要攻击动画播放从此槽位的武器
	 */
	FGameplayTag Equipment_Slot_MainHand;
    
	/** 
	 * 副手槽
	 * 含义：角色左手槽位，可装备盾牌或双持武器
	 * 应用：
	 *   - 盾牌：切换格挡动画，提升防守
	 *   - 双剑：解锁特殊的双武器招式
	 */
	FGameplayTag Equipment_Slot_OffHand;

	/**
	 * @section 防具槽位
	 * @brief   标识防具装备的槽位
	 */
	
	/** 
	 * 头盔槽
	 * 含义：头部防具，影响头部碰撞伤害
	 */
	FGameplayTag Equipment_Slot_Head;
    
	/** 
	 * 胸甲槽
	 * 含义：躯干防具，提供最多伤害减免
	 * 影响：改变角色负重
	 */
	FGameplayTag Equipment_Slot_Chest;
    
	/** 
	 * 手套槽
	 * 含义：手部防具，微幅影响攻击速度
	 */
	FGameplayTag Equipment_Slot_Gloves;
    
	/** 
	 * 腿甲槽
	 * 含义：腿部防具，影响移动速度
	 */
	FGameplayTag Equipment_Slot_Legs;

	
	// ========================================
	// 第九部分：武器类型 Tags（Weapon Type）
	// ========================================
	// 用途：标识武器的类型，决定可用的攻击招式和动画集
	// 规范：所有武器类型 Tag 以 Weapon_Type_ 前缀开头
	// 工作流程：装备武器 → 读取武器类型 Tag → 加载对应招式树
	
	/**
	 * @section 武器类型分类
	 * @brief   标识各种武器类型
	 * @注意事项
	 *   每个武器类型对应独立的攻击招式树（Combat Tree），包含 4-6 套连击动画。
	 *   更换武器时需要重新初始化当前连击链的状态。
	 */
	
	/** 
	 * 单手剑
	 * 含义：轻型剑类武器，平衡攻防
	 * 特性：
	 *   - 攻击速度：快
	 *   - 伤害：中等
	 *   - 特殊招式：刺击（突进、精准）
	 *   - 可双持（副手也装备单手剑）
	 * 招式数：6 套连击
	 */
	FGameplayTag Weapon_Type_Sword;
    
	/** 
	 * 巨剑/大剑
	 * 含义：重型剑类武器，单手操作
	 * 特性：
	 *   - 攻击速度：慢
	 *   - 伤害：高
	 *   - 特殊招式：重砍、挥舞（大范围）
	 *   - 需要较高的力量属性
	 *   - 不可双持
	 * 招式数：4 套连击
	 */
	FGameplayTag Weapon_Type_Greatsword;
    
	/** 
	 * 法杖
	 * 含义：魔法类武器，释放魔法技能
	 * 特性：
	 *   - 不进行物理近战攻击
	 *   - 触发法术 GA：火球、冰刺、龙吐息等
	 *   - 需要较高的智力属性
	 *   - 默认装备在双手（MainHand + OffHand）
	 * 招式数：无传统连击，改为法术快捷栏
	 */
	FGameplayTag Weapon_Type_Staff;


private:
	// ==================== 单例实例 ====================
	
	/** 
	 * @brief 全局唯一的 GameplayTags 单例实例
	 * @note 
	 *   静态存储期，在程序启动时构造，程序结束时析构。
	 *   InitializeNativeTags() 会填充其所有成员变量。
	 */
	static FZBGameplayTags GameplayTags;

	// ==================== 辅助方法 ====================

	/**
	 * @brief   注册单个 GameplayTag 到管理器
	 * @param   OutTag - 输出的 FGameplayTag 引用，接收注册后的 Tag
	 * @param   TagName - Tag 的全名（层级格式，如 "State.IFrame"）
	 * @param   TagComment - Tag 的描述文本，用于编辑器显示
	 * 
	 * @工作流程
	 *   1. 获取 UGameplayTagsManager 单例
	 *   2. 调用 RequestGameplayTag(TagName) 进行注册
	 *   3. 如果 Tag 已存在，返回已有的；否则创建新 Tag
	 *   4. 将注册后的 Tag 赋值给 OutTag 参数
	 * 
	 * @性能特性
	 *   - 首次调用：O(log n)，n 为已注册 Tag 总数（字典查询）
	 *   - 重复注册同一 Tag：O(1)，返回缓存结果
	 * 
	 * @错误处理
	 *   - 无效的 Tag 名字（包含非法字符）：编辑器会发出警告，Tag 可能无效
	 *   - 建议：仅在 InitializeNativeTags() 中调用，不在运行时调用
	 * 
	 * @注意事项
	 *   - TagName 必须遵循 UE 命名规范（英文字母、数字、下划线）
	 *   - TagComment 用于编辑器提示，应为中文描述
	 *   - Tag 注册后不可删除（游戏生命周期内持久存在）
	 */
	void AddTag(FGameplayTag& OutTag, const FName& TagName, const FString& TagComment);
};
