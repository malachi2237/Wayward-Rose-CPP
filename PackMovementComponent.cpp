// Fill out your copyright notice in the Description page of Project Settings.


#include "PackMovementComponent.h"

UPackMovementComponent::UPackMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UPackMovementComponent::AddInputVector(FVector WorldVector, bool bForce)
{
	UCharacterMovementComponent::AddInputVector(WorldVector * 5.0f, bForce);
}
