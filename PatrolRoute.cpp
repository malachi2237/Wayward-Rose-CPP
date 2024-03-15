// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolRoute.h"

// Sets default values
APatrolRoute::APatrolRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	
	if (WaypointList.Num() == 0)
		UE_LOG(LogTemp, Error, TEXT("Patrol Route Waypoint list may not be empty"));
}

// Called every frame
void APatrolRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APatrolRoute::CalculateNextIndex()
{
	switch (patrolType)
	{
	case Reverse:
		if (currentIndex + increment >= WaypointList.Num() ||
			currentIndex + increment < 0)
		{
			increment *= -1;
		}

		currentIndex += increment;
		break;
	case Loop:
		if (currentIndex + 1 >= WaypointList.Num())
		{
			currentIndex = 0;
			break;
		}

		currentIndex += increment;
		break;
	case Halt:
		if (currentIndex + 1 >= WaypointList.Num())
			break;
	}
}

FVector APatrolRoute::GetNextLocation()
{
	CalculateNextIndex();

	return WaypointList[currentIndex]->GetActorLocation();
}
