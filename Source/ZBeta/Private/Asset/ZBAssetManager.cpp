// Fill out your copyright notice in the Description page of Project Settings.


#include "Asset/ZBAssetManager.h"

#include "AbilitySystem/ZBGameplayTags.h"

UZBAssetManager& UZBAssetManager::Get()
{
	check(GEngine);
	return * Cast<UZBAssetManager>(GEngine->AssetManager);
}

void UZBAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FZBGameplayTags::InitializeNativeTags();
} 
