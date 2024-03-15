// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DebugChangeColor.generated.h"

#define DCC_REPLACED_MATERIAL_NAME TEXT("default")

struct MaterialNamePair {
	UMaterialInterface* material;
	FString name;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerFinishedEvent, FString, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerInterruptedEvent, FString, Name);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WAYWARDROSE_API UDebugChangeColor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDebugChangeColor();

	UPROPERTY(BlueprintAssignable)
	FTimerFinishedEvent OnTimerFinished;

	UPROPERTY(BlueprintAssignable)
	FTimerInterruptedEvent OnTimerInterrupted;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface *replacementMaterial;*/

	UFUNCTION(BlueprintCallable)
	void AssignMesh(UMeshComponent* mesh);

	/*DECLARE_EVENT_OneParam(UDebugChangeColor, FTimerFinishedEvent, bool)
	void OnTimerFinished() { return timerFinishedEvent; }*/

	//void OnTimerFinished_Implementation() {};
	/*UFUNCTION(BlueprintCallable)
	void AssignMeshes(const TArray<UMeshComponent*> &mesh, const bool overrideOthers);

	UFUNCTION(BlueprintCallable)
	void AssignMesh(UMeshComponent* mesh, const bool overrideOthers);*/

	

	UFUNCTION(BlueprintCallable)
	void ReplaceMaterials(UMaterialInterface* replacementMaterial, FString name, const float timerLength, const bool overrideOtherColors);

	UFUNCTION(BlueprintCallable)
	void RestoreMaterials(FString name, bool interrupt = false);


private:
	TArray<UMaterialInterface*> targetMeshMaterials;
	TArray<struct MaterialNamePair> materialBuffer;
	TArray<FTimerHandle> restoreColorTimerHandles;
	//TArray<UMeshComponent*> targetMeshes;
	UMeshComponent *targetMesh = nullptr;
	AActor *actor;

	
};
