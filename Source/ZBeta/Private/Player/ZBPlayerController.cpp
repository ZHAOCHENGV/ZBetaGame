// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZBPlayerController.h"

#include <rapidjson/internal/ieee754.h>

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
	if (ZBInputComponent && InputConfig)
	{
		// 原生输入绑定
		ZBInputComponent->BindAction(InputConfig->MoveAction,ETriggerEvent::Triggered,this,&AZBPlayerController::Input_Move);
		ZBInputComponent->BindAction(InputConfig->LookAction,ETriggerEvent::Triggered,this,&AZBPlayerController::Input_Look);
		ZBInputComponent->BindAction(InputConfig->ShiftAction,ETriggerEvent::Started,this,&AZBPlayerController::Input_Sprint_Started);
		ZBInputComponent->BindAction(InputConfig->ShiftAction,ETriggerEvent::Completed,this,&AZBPlayerController::Input_Sprint_Completed);
		ZBInputComponent->BindAction(InputConfig->MenuAction,ETriggerEvent::Started,this,&AZBPlayerController::Input_Menu);
		ZBInputComponent->BindAction(InputConfig->InteractionAction,ETriggerEvent::Started,this,&AZBPlayerController::Input_Interaction);
		ZBInputComponent->BindAction(InputConfig->TargetLockAction,ETriggerEvent::Started,this,&AZBPlayerController::Input_TargetLock);
		
		// 能力输入绑定
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
	UZBAbilitySystemComponent* ASC = GetASC();
	if (!ASC)
	{
		return;
	}
	ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(InputTag));
	UE_LOG(LogTemp, Log, TEXT("输入按下: %s"), *InputTag.ToString());
}

void AZBPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	UE_LOG(LogTemp, Log, TEXT("输入释放: %s"), *InputTag.ToString());
	
}

void AZBPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	UE_LOG(LogTemp, Log, TEXT("输入长按: %s"), *InputTag.ToString());
}


UZBAbilitySystemComponent* AZBPlayerController::GetASC()
{
	if (ZBAbilitySystemComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("未找到 ASC 组件，开始获取 ASC 组件"));  
		ZBAbilitySystemComponent = Cast<UZBAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	UE_LOG(LogTemp, Warning, TEXT("✓ 找到 ASC 组件：%s"),*ZBAbilitySystemComponent->GetName());
	return ZBAbilitySystemComponent;
}

void AZBPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	if (!GetASC())return;

	FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);// 前后
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);// 左右
	}
	
}

void AZBPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	FVector2D LookVector  = InputActionValue.Get<FVector2D>();
	AddYawInput(LookVector.X);    // 水平 → 左右旋转
	AddPitchInput(LookVector.Y);  // 垂直 → 上下旋转
}

void AZBPlayerController::Input_Sprint_Started()
{
	
}

void AZBPlayerController::Input_Sprint_Completed()
{
	
}

void AZBPlayerController::Input_Interaction()
{
	UE_LOG(LogTemp, Log, TEXT("按下交互键按键"));
}

void AZBPlayerController::Input_TargetLock()
{
	UE_LOG(LogTemp, Log, TEXT("按下锁定目标按键"));
}

void AZBPlayerController::Input_Menu()
{
	UE_LOG(LogTemp, Log, TEXT("按下菜单按键"));
}
