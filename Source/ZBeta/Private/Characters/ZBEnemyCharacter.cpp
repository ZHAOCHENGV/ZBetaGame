


#include "Characters/ZBEnemyCharacter.h"

#include "AbilitySystem/ZBAbilitySystemComponent.h"


AZBEnemyCharacter::AZBEnemyCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UZBAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");
}

void AZBEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
}

void AZBEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZBEnemyCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
}


void AZBEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AZBEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

