// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Waypoint.h"
#include "PatrolRoute.generated.h"

UENUM()
enum PatrolType
{
	Loop	UMETA(DisplayName = "Loop"),
	Reverse UMETA(DisplayName = "Reverse"),
	Halt	UMETA(DisplayName = "Halt")
};

UCLASS()
class WAYWARDROSE_API APatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolRoute();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PatrolOptions)
	TEnumAsByte<PatrolType> patrolType;

	UPROPERTY(EditInstanceOnly, Category = PatrolOptions)
	TArray<AWaypoint*> WaypointList;

	void CalculateNextIndex();

	int currentIndex = 0;
	int increment = 1;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FVector GetNextLocation();

};
