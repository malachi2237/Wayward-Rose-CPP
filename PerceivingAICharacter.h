// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaywardRoseAICharacter.h"

#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"

#include "PerceivingAICharacter.generated.h"

class UAIPerceptionComponent;

/**
 * 
 */
UCLASS(Abstract)
class WAYWARDROSE_API APerceivingAICharacter : public AWaywardRoseAICharacter
{
	GENERATED_BODY()

public:
	APerceivingAICharacter();

	void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	void DetectActor_Implementation(AActor* actor, const float strength) override;

	void LoseDetectedActor_Implementation(AActor* actor, const float maxAge) override;

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent *PerceptionComponent;

private:
	UPROPERTY(EditDefaultsOnly)
	FName HeadBoneName;

	UPROPERTY(EditDefaultsOnly)
	float MaxPerceptionDistance;

	UPROPERTY()
	AActor* DetectedActor = nullptr;

	UPROPERTY()
	FTimerHandle UpdatePerceptionStrengthTimerHandle;

	UFUNCTION()
	void UpdatePerceptionStrength();

	UFUNCTION()
	void PerceptionHandler(AActor *Actor, const FAIStimulus Stimulus);
};
