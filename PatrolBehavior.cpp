// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolBehavior.h"
#include "PatrolRoute.h"

// Sets default values for this component's properties
UPatrolBehavior::UPatrolBehavior()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPatrolBehavior::BeginPlay()
{
	if (PatrolRoute == nullptr)
		UE_LOG(LogTemp, Error, TEXT("Patrol Route is not set"));

	Super::BeginPlay();

	// ...
	
}


// Called every frame
//void UPatrolBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

FVector UPatrolBehavior::GetNextLocation()
{
	return PatrolRoute->GetNextLocation();
}