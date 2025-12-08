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
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ZBCharacterBase.generated.h"


class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;


// 标记为抽象类,防止在编辑器中直接实例化
UCLASS(Abstract)
class ZBETA_API AZBCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AZBCharacterBase();

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttribute() const { return AttributeSet; }

protected:

	virtual void BeginPlay() override;

	/**
	 * @brief 初始化 GAS 的 ActorInfo
	 * 这是一个虚函数，基类必须提供实现，哪怕是空的。
	 * 子类 (Player/Enemy) 会覆盖它来执行具体的初始化逻辑。
	 */
	virtual void InitAbilityActorInfo();

	//初始化主属性
	virtual void InitializePrimaryAttributes();

	// @brief 对自身应用一个 GameplayEffect（按指定等级 Level）
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GamePlayEffectClass,float Level);

	//GAS组件
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	//属性集
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	

	


private:
	
};
