// Fill out your copyright notice in the Description page of Project Settings.


#include "WaywardRoseAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AWaywardRoseAICharacter::AWaywardRoseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLocation = GetActorLocation();
	GetCharacterMovement()->bOrientRotationToMovement = true;
}


void AWaywardRoseAICharacter::EnterPatrolState_Implementation()
{
	auto movementComp = GetCharacterMovement();

	movementComp->MaxWalkSpeed = WalkSpeed;
	movementComp->RotationRate = FRotator(0.0, WalkRotationRate, 0.0);
}

void AWaywardRoseAICharacter::EnterChaseState_Implementation()
{
	auto movementComp = GetCharacterMovement();

	movementComp->MaxWalkSpeed = RunSpeed;
	movementComp->RotationRate = FRotator(0.0, RunRotationRate, 0.0);
}

void AWaywardRoseAICharacter::EnterSearchState_Implementation()
{
	auto movementComp = GetCharacterMovement();

	movementComp->MaxWalkSpeed = SearchSpeed;
	movementComp->RotationRate = FRotator(0.0, SearchRotationRate, 0.0);
}

FVector AWaywardRoseAICharacter::GetNextPatrolLocation_Implementation()
{
	return InitialLocation;
}

void AWaywardRoseAICharacter::AttackActor_Implementation(AActor *target)
{
	UGameplayStatics::ApplyDamage(target, Damage, GetController(), this, UDamageType::StaticClass());
}

// Called when the game starts or when spawned
void AWaywardRoseAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWaywardRoseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaywardRoseAICharacter::DetectActor_Implementation(AActor* actor, const float strength)
{
	OnActorDetected.Broadcast(actor, strength);
}

void AWaywardRoseAICharacter::LoseDetectedActor_Implementation(AActor* actor, const float maxAge)
{
	OnActorExitDetection.Broadcast(actor, maxAge);
}
