/**
* @brief 游戏角色基类 - 所有可操作角色的抽象父类
 * 
 * @details 
 * 这是一个抽象基类,为游戏中所有角色(玩家、敌人、NPC)提供统一的基础框架。
 * 集成了GAS(Gameplay Ability System)核心组件,为技能系统、属性系统提供支持。
 * 
 * **核心功能:**
 * - 集成AbilitySystemComponent,支持GAS技能系统
 * - 集成AttributeSet,管理角色属性(生命值、耐力等)
 * - 提供Enhanced Input的输入绑定接口
 * - 作为抽象类,强制子类实现具体角色逻辑
 * 
 * **架构设计:**
 * - 使用UCLASS(Abstract)确保不能直接实例化
 * - 遵循UE5组件化设计模式
 * - 为PlayerCharacter、EnemyCharacter等子类提供统一接口
 * 
 * **GAS集成要点:**
 * - AbilitySystemComponent需在子类中初始化并注册
 * - AttributeSet需要配置对应的GameplayEffect来修改属性
 * - 需要在合适的时机调用InitAbilityActorInfo()
 * 
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;


// 标记为抽象类,防止在编辑器中直接实例化
UCLASS(Abstract)
class ZBETA_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	ACharacterBase();

	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 设置玩家输入组件绑定
	 * @param PlayerInputComponent 输入组件指针
	 * @details 
	 * Enhanced Input系统的输入绑定入口点。
	 * 应在此函数中绑定InputAction到对应的回调函数。
	 * 
	 * **典型实现流程:**
	 * 1. 获取EnhancedInputComponent
	 * 2. 绑定InputAction(移动、跳跃、攻击等)
	 * 3. 关联到具体的处理函数
	 * 
	 * @warning 仅在拥有PlayerController的角色上调用
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;


	/**
	 * @brief Gameplay Ability System 核心组件
	 * @details 
	 * 负责管理角色的所有Gameplay Ability(技能):
	 * - 技能的授予(GrantAbility)
	 * - 技能的激活(TryActivateAbility)
	 * - 技能的取消和结束
	 * - GameplayEffect的应用
	 * - GameplayTag的管理
	 * - GameplayCue的触发
	 * 
	 * **重要接口:**
	 * - InitAbilityActorInfo(): 初始化ASC与Actor的关联
	 * - GiveAbility(): 授予技能
	 * - ApplyGameplayEffectToSelf(): 应用效果到自身
	 * 
	 * @warning 必须在子类构造函数中使用CreateDefaultSubobject创建
	 * @warning 需要在BeginPlay或合适时机调用InitAbilityActorInfo()
	 * @note 对于玩家角色,通常在PlayerState中创建ASC以支持网络同步
	 * @note 对于AI角色,通常直接在Character中创建ASC
	 */
	UPROPERTY()// 标记为UProperty以支持垃圾回收和序列化
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;


	/**
	 * @brief 角色属性集合
	 * @details 
	 * 存储角色的所有Gameplay属性,如:
	 * - 生命值(Health)、最大生命值(MaxHealth)
	 * - 耐力(Stamina)、最大耐力(MaxStamina)
	 * - 攻击力(AttackPower)、防御力(Defense)
	 * - 移动速度(MovementSpeed)
	 * 
	 * **使用方式:**
	 * - 通过GameplayEffect修改属性值
	 * - 使用AttributeSet的宏定义属性(ATTRIBUTE_ACCESSORS)
	 * - 监听属性变化事件(OnHealthChanged等)
	 * 
	 * **网络同步:**
	 * - 属性需要标记为Replicated才能在多人游戏中同步
	 * - 使用GetLifetimeReplicatedProps()配置复制规则
	 * 
	 * @warning 必须在子类构造函数中使用CreateDefaultSubobject创建
	 * @warning AttributeSet需要配合ASC使用,不能单独工作
	 * @note 建议为不同角色类型(玩家/敌人)创建不同的AttributeSet子类
	 */
	UPROPERTY()// 标记为UProperty以支持垃圾回收和序列化
	TObjectPtr<UAttributeSet> AttributeSet;

};
