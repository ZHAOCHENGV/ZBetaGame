// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "ZBInputConfig.h"
#include "ZBEnhancedInputComponent.generated.h"

/**
 * @file    ZBEnhancedInputComponent.h
 * @brief   增强输入组件 - 将 Enhanced Input System 与 GAS 能力系统集成
 * 
 * @核心功能
 *   将玩家的输入操作（Input Action）与游戏能力（Gameplay Ability）绑定。
 *   支持按下、释放、持续按住三种输入事件，并将对应的 InputTag 传递给能力系统。
 * 
 * @设计意图
 *   1. 简化能力系统的输入绑定流程
 *   2. 通过模板实现类型安全的回调函数绑定
 *   3. 将配置数据驱动的输入映射与代码解耦
 * 
 * @继承关系
 *   UZBEnhancedInputComponent → UEnhancedInputComponent → UInputComponent
 *   （这里继承虚幻原生的 Enhanced Input Component）
 */

// 分类ClassGroup=(Custom),   在蓝图可创建组件meta=(BlueprintSpawnableComponent),
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZBETA_API UZBEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	/**
	 * @brief   绑定能力系统的输入回调函数
	 * 
	 * @template_param UserClass - 回调所有者的类类型（通常是 Pawn/Character）
	 * @template_param PressedFuncType - 按下事件的回调函数类型
	 * @template_param ReleasedFuncType - 释放事件的回调函数类型
	 * @template_param HeldFuncType - 持续按住事件的回调函数类型
	 * 
	 * @param    InputConfig - 输入配置数据，包含所有 Input Action 和 Tag 映射
	 * @param    Object - 回调函数的所有者对象（通常是玩家控制的 Pawn）
	 * @param    PressedFunc - 输入按下时的回调函数指针
	 * @param    ReleasedFunc - 输入释放时的回调函数指针
	 * @param    HeldFunc - 输入持续按住时的回调函数指针
	 * 
	 * @工作流程
	 *   1. 遍历 InputConfig 中的所有输入配置项
	 *   2. 检验 InputAction 和 InputTag 的有效性
	 *   3. 对每个有效的输入配置创建三个事件绑定（按下/释放/持续）
	 *   4. 当玩家触发输入时，自动调用对应的回调函数并传递 InputTag
	 * 
	 * @参数说明
	 *   InputConfig：
	 *     - 数据驱动配置对象
	 *     - 包含 TArray<FZBInputAction> AbilityInputActions
	 *     - 每个 FZBInputAction 包含：
	 *       ├─ UInputAction* InputAction（虚幻输入系统中的输入动作）
	 *       └─ FGameplayTag InputTag（对应的 GameplayTag，标识能力类型）
	 * 
	 *   Object：
	 *     - 回调函数的执行对象
	 *     - 通常为 ACharacter 或其子类
	 *     - 必须存活到输入事件触发时
	 * 
	 *   PressedFunc/ReleasedFunc/HeldFunc：
	 *     - 函数指针，指向 Object 中的成员函数
	 *     - 签名必须为：void FunctionName(const FGameplayTag& InputTag)
	 *     - 示例：&AZBCharacter::HandleAbilityInputPressed
	 * 
	 * @绑定时机
	 *   - 调用时机：PlayerController 初始化输入时（SetupPlayerInputComponent）
	 *   - 绑定生命周期：与 InputComponent 相同（通常为角色生命周期）
	 * 
	 * @多模板参数的设计优势
	 *   1. 类型安全：编译期检查函数签名
	 *   2. 灵活性：支持不同类类型的对象
	 *   3. 性能：零运行时类型检查开销（编译期确定）
	 *   4. 可选性：可以只绑定需要的回调，传递 nullptr 表示跳过
	 * 
	 * @注意事项
	 *   - 如果 PressedFunc 为 nullptr，则按下事件不会触发回调
	 *   - InputConfig 必须有效，否则 check() 会触发断言
	 *   - 回调函数必须在 Object 销毁前完成执行
	 *   - 同一个 InputAction 如果绑定多次会产生多个回调（可能不是期望行为）
	 */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UZBInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};



/**
 * @brief   模板函数实现
 * 
 * @实现细节分解
 * 
 * 第一步：验证输入配置
 *   check(InputConfig);
 *   - 运行时断言：确保 InputConfig 不为空指针
 *   - 在发布版本中编译时移除，仅开发阶段有效
 *   - 若为空则直接崩溃，防止后续访问空指针
 * 
 * 第二步：遍历所有输入映射
 *   for (const FZBInputAction& Action : InputConfig->AbilityInputActions)
 *   - 遍历配置中的每一个输入动作映射
 *   - FZBInputAction 是自定义结构体，包含：
 *     ├─ UInputAction* InputAction（Enhanced Input 的输入动作）
 *     └─ FGameplayTag InputTag（GAS 中对应的能力标签）
 *   - const 引用避免不必要的复制，性能最优
 * 
 * 第三步：有效性检查
 *   if (Action.InputAction && Action.InputTag.IsValid())
 *   - 确保 InputAction 指针有效（非空）
 *   - 确保 InputTag 已正确初始化（非空标签）
 *   - 跳过任何不完整的配置，防止无效绑定
 * 
 * 第四步：条件性绑定三种输入事件
 * 
 *   【事件类型 1：按下事件】
 *   if (PressedFunc)
	 *   {
	 *       BindAction(
	 *           Action.InputAction,           // 输入动作（如 IA_Attack）
	 *           ETriggerEvent::Started,       // 触发事件（按下时触发一次）
	 *           Object,                       // 回调函数所有者
	 *           PressedFunc,                  // 回调函数指针
	 *           Action.InputTag               // 传递给回调的参数（InputTag）
	 *       );
	 *   }
	 * 
	 * @按下事件详解
	 *   - ETriggerEvent::Started：输入刚开始按下（按下的第一帧）
	 *   - 应用场景：
	 *     ├─ 激活轻攻击能力
	 *     ├─ 启动冲刺
	 *     └─ 开始格挡蓄力
	 *   - 回调签名：void PressedFunc(const FGameplayTag& InputTag)
	 *   - Action.InputTag 作为参数自动传递，标识是哪个输入触发的
	 * 
	 *   【事件类型 2：释放事件】
	 *   if (ReleasedFunc)
	 *   {
	 *       BindAction(
	 *           Action.InputAction,
	 *           ETriggerEvent::Completed,     // 触发事件（按键释放时触发）
	 *           Object,
	 *           ReleasedFunc,
	 *           Action.InputTag
	 *       );
	 *   }
	 * 
	 * @释放事件详解
	 *   - ETriggerEvent::Completed：输入刚释放（按键松开的时刻）
	 *   - 应用场景：
	 *     ├─ 重攻击的蓄力释放
	 *     ├─ 停止持续冲刺
	 *     └─ 取消格挡状态
	 *   - 配合 Started 和 Triggered 实现完整的输入生命周期
	 * 
	 *   【事件类型 3：持续事件】
	 *   if (HeldFunc)
	 *   {
	 *       BindAction(
	 *           Action.InputAction,
	 *           ETriggerEvent::Triggered,     // 触发事件（按下期间每帧触发）
	 *           Object,
	 *           HeldFunc,
	 *           Action.InputTag
	 *       );
	 *   }
	 * 
	 * @持续事件详解
	 *   - ETriggerEvent::Triggered：输入持续按住时的每帧回调
	 *   - 应用场景：
	 *     ├─ 实时检测冲刺距离
	 *     ├─ 更新格挡受伤计算
	 *     └─ 监测蓄力时间（重攻击）
	 *   - 每帧调用一次，直到输入释放
	 * 
	 * @三种事件的时间线示意
	 * 
	 *   时间线：
	 *   Frame 0：按键按下
	 *            ↓
	 *            Started → PressedFunc() 被调用一次
	 * 
	 *   Frame 1-N：按键保持按下
	 *            ↓
	 *            Triggered → HeldFunc() 每帧调用一次
	 * 
	 *   Frame N+1：按键释放
	 *            ↓
	 *            Completed → ReleasedFunc() 被调用一次
	 * 
	 * @实际场景应用
	 * 
	 * 【场景 1：轻攻击（短按）】
	 *   // 输入配置
	 *   PressedFunc = &ACharacter::OnAttackPressed
	 *   ReleasedFunc = nullptr（不需要）
	 *   HeldFunc = nullptr（不需要）
	 *   
	 *   // 执行流程
	 *   玩家短按→Started 触发→OnAttackPressed(InputTag::Attack_Light)
	 *   →触发轻攻击能力
	 * 
	 * 【场景 2：重攻击（长按蓄力）】
	 *   // 输入配置
	 *   PressedFunc = nullptr（不需要）
	 *   ReleasedFunc = &ACharacter::OnAttackReleased
	 *   HeldFunc = &ACharacter::OnAttackHeld
	 *   
	 *   // 执行流程
	 *   玩家长按→Started（跳过）
	 *        →Triggered 每帧→OnAttackHeld(InputTag::Attack_Heavy)
	 *          更新蓄力UI 进度条
	 *        →松开按键→Completed→OnAttackReleased(InputTag::Attack_Heavy)
	 *          根据蓄力时间计算伤害并释放重攻击
	 * 
	 * 【场景 3：冲刺（持续加速）】
	 *   // 输入配置
	 *   PressedFunc = &ACharacter::OnSprintPressed
	 *   ReleasedFunc = &ACharacter::OnSprintReleased
	 *   HeldFunc = &ACharacter::OnSprintHeld
	 *   
	 *   // 执行流程
	 *   玩家按 Shift→Started→OnSprintPressed(InputTag::Sprint)
	 *             →激活冲刺能力，加速
	 *             →Triggered 每帧→OnSprintHeld(InputTag::Sprint)
	 *             →更新粒子效果、消耗耐力
	 *             →松开 Shift→Completed→OnSprintReleased(InputTag::Sprint)
	 *             →停止冲刺，恢复正常速度
	 * 
	 * @模板参数的高级特性
	 * 
	 * 为什么使用多个 FuncType 模板参数？
	 *   1. 编译期类型检查：
	 *      - PressedFunc、ReleasedFunc、HeldFunc 可以有不同的签名
	 *      - 但实际上它们应该都是 void(*)(const FGameplayTag&) 的变体
	 *      - 编译器会在模板实例化时验证
	 * 
	 *   2. 支持 nullptr：
	 *      - if (PressedFunc) 检查允许传递 nullptr
	 *      - 这样可以选择性地只绑定需要的事件
	 *      - 避免不必要的回调创建
	 * 
	 *   3. 性能优化：
	 *      - 模板会在编译期展开成具体代码
	 *      - 比虚函数/委托机制性能更高
	 *      - 零运行时多态开销
	 * 
	 * @错误处理与边界条件
	 * 
	 * 1. InputConfig 为空
	 *    → check() 触发断言，程序停止
	 *    → 在发布版本中移除此检查
	 * 
	 * 2. InputAction 为空
	 *    → if 条件跳过，静默忽略
	 *    → 可能导致输入无响应，需配置检查
	 * 
	 * 3. InputTag 无效
	 *    → if 条件跳过，静默忽略
	 *    → 同样可能导致能力无法触发
	 * 
	 * 4. Object 被销毁但输入仍存在
	 *    → 访问空指针导致崩溃
	 *    → 解决方案：在 Object 析构前移除输入组件
	 * 
	 * @与 GAS 的集成方式
	 * 
	 * 完整的能力触发流程：
	 * 
	 *   输入按下
	 *     ↓
	 *   Enhanced Input 识别并触发 InputAction
	 *     ↓
	 *   BindAbilityActions 回调对应函数
	 *     ↓
	 *   OnAbilityInputPressed(InputTag)
	 *     ↓
	 *   查找 ASC 中对应的 Ability
	 *     ↓
	 *   ASC→TryActivateAbilityByTag(InputTag)
	 *     ↓
	 *   GAS 激活能力，执行 GA 逻辑
	 *     ↓
	 *   能力执行完成或被中断
	 *     ↓
	 *   输入释放
	 * 
	 * @使用示例
	 * 
	 * // 在 Pawn 的 SetupPlayerInputComponent 中：
	 * void AZBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
	 * {
	 *     Super::SetupPlayerInputComponent(PlayerInputComponent);
	 *     
	 *     // 获取增强输入组件
	 *     if (UZBEnhancedInputComponent* EnhancedInputComponent = 
	 *         CastChecked<UZBEnhancedInputComponent>(PlayerInputComponent))
	 *     {
	 *         // 绑定所有能力输入
	 *         EnhancedInputComponent->BindAbilityActions(
	 *             InputConfigData,                      // 输入配置
	 *             this,                                 // 回调所有者
	 *             &AZBCharacter::OnAbilityInputPressed,  // 按下回调
	 *             &AZBCharacter::OnAbilityInputReleased, // 释放回调
	 *             &AZBCharacter::OnAbilityInputHeld      // 持续回调
	 *         );
	 *     }
	 * }
	 * 
	 * void AZBCharacter::OnAbilityInputPressed(const FGameplayTag& InputTag)
	 * {
	 *     // 当输入按下时，尝试激活对应的能力
	 *     AbilitySystemComponent->TryActivateAbilityByTag(InputTag);
	 * }
	 * 
	 * void AZBCharacter::OnAbilityInputReleased(const FGameplayTag& InputTag)
	 * {
	 *     // 当输入释放时，可能需要停止持续型能力
	 *     AbilitySystemComponent->CancelAbilityByTag(InputTag);
	 * }
	 * 
	 * void AZBCharacter::OnAbilityInputHeld(const FGameplayTag& InputTag)
	 * {
	 *     // 当输入持续按住时，可以更新能力状态
	 *     // 例如更新蓄力进度、消耗资源等
	 * }
	 * 
	 * @性能考虑
	 * 
	 * 1. 回调开销：O(1)，直接函数指针调用
	 * 2. 配置遍历：O(n)，n 为输入映射数量（通常 10-20）
	 * 3. 绑定一次性开销：在初始化时执行，不在 Tick 中重复
	 * 4. 运行时开销：零额外开销（仅是函数调用）
	 * 
	 * @与其他输入系统的对比
	 * 
	 * 旧 InputComponent 方式：
	 *   - 字符串绑定，运行时类型不安全
	 *   - 无法传递自定义参数（难以实现多个输入映射）
	 *   - 代码重复多，维护困难
	 * 
	 * 本方案优势：
	 *   - 编译期类型安全
	 *   - 灵活的参数传递（InputTag）
	 *   - 数据驱动（通过 InputConfig 配置）
	 *   - 支持三种事件完整生命周期
	 *   - 代码简洁，易于维护
	 */
template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UZBEnhancedInputComponent::BindAbilityActions(const UZBInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	// 🔧 验证配置有效性
	check(InputConfig);
	
	// 🔧 遍历所有输入映射配置
	for (const FZBInputAction& Action : InputConfig->AbilityInputActions)
	{
		// 🔧 验证当前配置项完整性
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			// ✨ 绑定【按下事件】- 输入开始时触发一次
			if (PressedFunc)
			{
				BindAction(
					Action.InputAction,           // 输入动作
					ETriggerEvent::Started,       // 触发事件类型：按下
					Object,                       // 回调所有者
					PressedFunc,                  // 回调函数
					Action.InputTag               // 参数：输入标签
				);
			}

			// ✨ 绑定【释放事件】- 输入结束时触发一次
			if (ReleasedFunc)
			{
				BindAction(
					Action.InputAction,
					ETriggerEvent::Completed,     // 触发事件类型：释放
					Object,
					ReleasedFunc,
					Action.InputTag
				);
			}
		
			// ✨ 绑定【持续事件】- 输入按住期间每帧触发
			if (HeldFunc)
			{
				BindAction(
					Action.InputAction,
					ETriggerEvent::Triggered,     // 触发事件类型：持续
					Object,
					HeldFunc,
					Action.InputTag
				);
			}
		}
	}
}
