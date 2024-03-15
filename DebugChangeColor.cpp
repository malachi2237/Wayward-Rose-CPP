// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugChangeColor.h"

// Sets default values for this component's properties
UDebugChangeColor::UDebugChangeColor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDebugChangeColor::BeginPlay()
{
	Super::BeginPlay();
	
	AActor *temp = GetOwner();

	if (temp != nullptr)
		actor = temp;

	// ...
	
}

void UDebugChangeColor::AssignMesh(UMeshComponent* mesh)
{
	if (mesh == nullptr || targetMesh != nullptr)
		return;

	targetMesh = mesh;
	targetMeshMaterials = TArray<UMaterialInterface*>(mesh->GetMaterials());
	//targetMeshes.Add(mesh);
}
//void UDebugChangeColor::AssignMesh(UMeshComponent* mesh, const bool overrideOthers)
//{
//	if (overrideOthers)
//		targetMeshes.Empty();
//	
//	AssignMesh(mesh);
//}
//
//void UDebugChangeColor::AssignMeshes(const TArray<UMeshComponent*> &meshes, const bool overrideOthers = true)
//{
//	if (overrideOthers)
//		targetMeshes.Empty();
//
//	for (int i = 0; i < meshes.Num(); i++)
//		AssignMesh(meshes[i]);
//}

void UDebugChangeColor::ReplaceMaterials(UMaterialInterface* replacementMaterial, FString name, const float timerLength, const bool overrideOtherColors = true)
{
	if (targetMesh == nullptr)
		return;

	FTimerHandle newTimer;
	struct MaterialNamePair replacementMaterialPair = { replacementMaterial, name };

	if (overrideOtherColors)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

		for (int i = 0; i < materialBuffer.Num(); i++)
			OnTimerInterrupted.Broadcast(materialBuffer[i].name);

		materialBuffer.Empty();
	}

	for (int i = 0; i < targetMeshMaterials.Num(); i++)
		targetMesh->SetMaterial(i, replacementMaterial);

	materialBuffer.Add(replacementMaterialPair);

	GetWorld()->GetTimerManager().SetTimer(newTimer, FTimerDelegate::CreateUObject(this, &UDebugChangeColor::RestoreMaterials, name, false), timerLength, false);

}

void UDebugChangeColor::RestoreMaterials(FString name, bool interrupt)
{
	if (targetMesh == nullptr)
		return;

	int expiredMatIndex = materialBuffer.FindLastByPredicate([name](struct MaterialNamePair m) { return m.name == name; });

	if (expiredMatIndex != INDEX_NONE)
	{
		//struct MaterialNamePair expiredMaterial = materialBuffer[expiredMatIndex];

		materialBuffer.RemoveAt(expiredMatIndex);

		if (materialBuffer.Num() > 0)
		{
			for (int i = 0; i < targetMeshMaterials.Num(); i++)
				targetMesh->SetMaterial(i, materialBuffer.Top().material);
		}
		else
		{
			for (int i = 0; i < targetMeshMaterials.Num(); i++)
				targetMesh->SetMaterial(i, targetMeshMaterials[i]);
		}

		if (interrupt)
			OnTimerInterrupted.Broadcast(name);
		else
			OnTimerFinished.Broadcast(name);
	}
}
