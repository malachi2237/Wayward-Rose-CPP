// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolBehavior.generated.h"

class APatrolRoute;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WAYWARDROSE_API UPatrolBehavior : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolBehavior();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	struct FVector InitialLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	APatrolRoute *PatrolRoute;


public:	
	//// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FVector GetNextLocation();
};
