// Fill out your copyright notice in the Description page of Project Settings.


#include "PerceivingAICharacter.h"
#include "TimerManager.h"

#define APAIC_STRENGTH_UPDATE_INTERVAL 0.25f

APerceivingAICharacter::APerceivingAICharacter()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &APerceivingAICharacter::PerceptionHandler);
}

void APerceivingAICharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetMesh()->GetSocketLocation(HeadBoneName);

	OutRotation = GetActorRotation();
	OutRotation.Yaw -= GetMesh()->GetSocketTransform(HeadBoneName, RTS_ParentBoneSpace).Rotator().Roll;
}

void APerceivingAICharacter::PerceptionHandler(AActor* Actor, const FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		float strength = 1.0f - FVector::Dist(Stimulus.ReceiverLocation, Stimulus.StimulusLocation) / MaxPerceptionDistance;

		strength = FMath::Clamp(strength, 0.1f, 1.0f);

		DetectActor(Actor, strength);
	}
	else
		LoseDetectedActor(Actor, TimeToLoseActors);
}

void APerceivingAICharacter::UpdatePerceptionStrength()
{
	float strength;

	if (DetectedActor)
	{
		strength = 1.0f - FVector::Dist(K2_GetActorLocation(), DetectedActor->GetActorLocation()) / MaxPerceptionDistance;
		strength = FMath::Clamp(strength, 0.1f, 1.0f);

		Super::DetectActor_Implementation(DetectedActor, strength);
	}
}

void APerceivingAICharacter::DetectActor_Implementation(AActor* actor, const float strength)
{
	DetectedActor = actor;

	if (GetWorld())
		GetWorldTimerManager().SetTimer(UpdatePerceptionStrengthTimerHandle, this, &APerceivingAICharacter::UpdatePerceptionStrength,
		APAIC_STRENGTH_UPDATE_INTERVAL, true, 0.0f);

	Super::DetectActor_Implementation(actor, strength);
}

void APerceivingAICharacter::LoseDetectedActor_Implementation(AActor* actor, const float maxAge)
{
	DetectedActor = nullptr;

	if (GetWorld())
		GetWorldTimerManager().ClearTimer(UpdatePerceptionStrengthTimerHandle);

	Super::LoseDetectedActor_Implementation(actor, maxAge);
}