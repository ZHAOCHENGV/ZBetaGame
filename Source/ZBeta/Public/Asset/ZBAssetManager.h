// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ZBAssetManager.generated.h"

/**
 * 游戏的资源管理器
 * 负责在引擎启动时加载全局数据（如 Native GameplayTags）
 */
UCLASS()
class ZBETA_API UZBAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	// 获取单例的静态方法
	static UZBAssetManager& Get();
	
private:
	/** 引擎初始化加载数据时会调用此函数 */
	virtual void StartInitialLoading() override;
	
};
