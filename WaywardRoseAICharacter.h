// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WaywardRoseAICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorEnterDetectionDelegate, AActor*, DetectedActor, const float, PerceptionStrength);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorExitDetection, AActor*, DetectedActor, const float, MaxAge);

/** The base class for AI characters in Wayward Rose. Establishes interfaces for Controllers to control state. */
UCLASS(Abstract, Blueprintable)
class WAYWARDROSE_API AWaywardRoseAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWaywardRoseAICharacter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnterPatrolState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnterSearchState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnterChaseState();

	UFUNCTION(BlueprintImplementableEvent)
	void EnterDetectingState();

	UFUNCTION(BlueprintImplementableEvent)
	void ExitDetectingState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetNextPatrolLocation();

	/** Deal damage to a target actor. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackActor(AActor *target);

	UPROPERTY(EditAnywhere)
	float TimeToDetectActors;

	UPROPERTY(EditAnywhere)
	float TimeToLoseActors;
	/** The amount of time the character should wait after reaching a patrol point*/
	UPROPERTY(EditAnywhere)
	float PatrolWaitTime;

	/** The maximum distance around a patrol point this character will travel in a patrol state */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PatrolRadius;

	/** Delegate for when an actor is detected by this AI */
	UPROPERTY(BlueprintAssignable)
	FOnActorEnterDetectionDelegate OnActorDetected;

	/** Delegate for when an actor that was being detected is no longer being detected by this AI. */
	UPROPERTY(BlueprintAssignable)
	FOnActorExitDetection OnActorExitDetection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DetectActor(AActor* actor, const float strength);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoseDetectedActor(AActor* actor, const float maxAge);

	/** The amount of damage dealt to an Actor when AttackActor is called */
	UPROPERTY(EditAnywhere)
	float Damage = 1.0f;


	UPROPERTY(EditAnywhere, Category = "MovementRates")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = "MovementRates")
	float RunSpeed;

	UPROPERTY(EditAnywhere, Category = "MovementRates")
	float SearchSpeed;

	UPROPERTY(EditAnywhere, Category = "MovementRates")
	float WalkRotationRate;

	UPROPERTY(EditAnywhere, Category = "MovementRates")
	float RunRotationRate;

	UPROPERTY(EditAnywhere, Category = "MovementRates")
	float SearchRotationRate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	FVector InitialLocation;
};
