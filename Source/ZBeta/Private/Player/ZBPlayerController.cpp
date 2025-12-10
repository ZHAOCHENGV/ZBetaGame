// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZBPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/ZBAbilitySystemComponent.h"
#include "Input/ZBEnhancedInputComponent.h"

AZBPlayerController::AZBPlayerController()
{
	bReplicates = true;
}

void AZBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 不崩溃，只警告
	ensureMsgf(DefaultInputMappingContext, TEXT("DefaultInputMappingContext 没有设置！"));

	// 获取增强输入系统 LocalPlayerSubsystem（PlayerController 才能正确 GetLocalPlayer）
	UEnhancedInputLocalPlayerSubsystem * Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInput Subsystem 获取成功!!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInput Subsystem 获取失败!!!"));
	}
	
	// 设置鼠标/输入模式
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void AZBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UZBEnhancedInputComponent* ZBInputComponent = CastChecked<UZBEnhancedInputComponent>(InputComponent);
	if (ZBInputComponent)
	{
		ZBInputComponent->BindAbilityActions(
			InputConfig,
			this,
			&ThisClass::AbilityInputPressed,
			&ThisClass::AbilityInputReleased,
			&ThisClass::AbilityInputHeld);
	}
	
	
}

void AZBPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
}

void AZBPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
}

void AZBPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
}


UZBAbilitySystemComponent* AZBPlayerController::GetASC()
{
	if (ZBAbilitySystemComponent == nullptr)
	{
		ZBAbilitySystemComponent = Cast<UZBAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return ZBAbilitySystemComponent;
}
