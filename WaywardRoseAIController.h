// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DetourCrowdAIController.h"

#include "WaywardRoseAIController.generated.h"

class AWaywardRoseAICharacter;
class UBehaviorTreeComponent;
class UBlackboardComponent;

UENUM()
enum class EDetectionState : int32
{
	Detecting,
	Detected,
	LosingFromDetecting,
	LosingFromDetected,
	Searching,
	None
};

/** AI Controller intended for use with WaywardRoseAICharacter subclasses. */
UCLASS(Abstract)
class WAYWARDROSE_API AWaywardRoseAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWaywardRoseAIController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	void OnPossess(APawn* InPawn) override;

	void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void ForceLoseTarget();

	
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComponent;

protected:
	void TransitionToSearchState();
	void TransitionToDetectedState();
	void TransitionToDetectingState();
	void TransitionToNoneState();

	void TransitionFromSearchState();
	void TransitionFromDetectedState();
	void TransitionFromDetectingState();

	UFUNCTION()
	void BeginPerceivingActor(AActor* actor, const float strength);

	UFUNCTION()
	void ProcessTargetLost(AActor* actor, const float MaxAge);

	UPROPERTY()
	AWaywardRoseAICharacter* PossessedCharacter;

	UPROPERTY()
	AActor* DetectedActor = nullptr;

	float CurrentPerceptionStrength = 0.0f;

	float DetectionTimer = 0.0f;

	float UndetectTimer = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxSearchTime;

	float SearchTimer = 0.0f;

	EDetectionState DetectionState = EDetectionState::None;

	UPROPERTY(EditDefaultsOnly, Category = "KeyNames")
	FName TargetActorKey;

	UPROPERTY(EditDefaultsOnly, Category = "KeyNames")
	FName TargetLocationKey;

	UPROPERTY(EditDefaultsOnly, Category = "KeyNames")
	FName PatrolWaitTimeKey;

	UPROPERTY(EditDefaultsOnly, Category = "KeyNames")
	FName CharacterIsIntangibleKey;

	UPROPERTY(EditDefaultsOnly, Category = "KeyNames")
	FName DetectingTargetKey;
};
