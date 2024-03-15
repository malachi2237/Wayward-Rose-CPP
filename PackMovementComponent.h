// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PackMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WAYWARDROSE_API UPackMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		struct FVector tempTargetLocation;
	UPROPERTY(BlueprintReadWrite)
		float distanceFromPackMembers;
	UPROPERTY()
		float repulsionInfluence;
	UPROPERTY()
		float cohesionInfluence;
	UPROPERTY()
		float centerInfluence;
	UPROPERTY()
		float travelDirectionInfluence;
public:
	UPackMovementComponent();
	void AddInputVector(FVector WorldVector, bool bForce) override;
	/*virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;*/

};
