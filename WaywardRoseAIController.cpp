// Fill out your copyright notice in the Description page of Project Settings.


#include "WaywardRoseAIController.h"
#include "WaywardRoseAICharacter.h"

#include "GameFramework/Character.h"

#include "Navigation/CrowdFollowingComponent.h" 

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"

AWaywardRoseAIController::AWaywardRoseAIController(const FObjectInitializer& ObjectInitializer) :
	AAIController(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AWaywardRoseAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);
}

void AWaywardRoseAIController::Tick(float DeltaSeconds)
{
	switch (DetectionState)
	{
		case EDetectionState::Detecting:
			if (DetectionTimer >= PossessedCharacter->TimeToDetectActors)
			{
				TransitionFromDetectingState();
				TransitionToDetectedState();
			}
			else
				DetectionTimer += CurrentPerceptionStrength * DeltaSeconds;

			break;
		case EDetectionState::LosingFromDetecting:
			if (UndetectTimer >= PossessedCharacter->TimeToLoseActors)
			{
				TransitionFromDetectingState();
				TransitionToNoneState();

				DetectedActor = nullptr;
			}
			else
				UndetectTimer += DeltaSeconds;

			break;
		case EDetectionState::LosingFromDetected:
			if (UndetectTimer >= PossessedCharacter->TimeToLoseActors)
			{
				TransitionFromDetectedState();
				TransitionToSearchState();
				
				DetectedActor = nullptr;
			}
			else
				UndetectTimer += DeltaSeconds;

			break;
		case EDetectionState::Searching:
			if (SearchTimer >= MaxSearchTime)
			{
				TransitionFromSearchState();

				TransitionToNoneState();
				PossessedCharacter->EnterPatrolState();
			}
			else
				SearchTimer += DeltaSeconds;

			break;
		default:
			break;
	}
}

void AWaywardRoseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedCharacter = Cast<AWaywardRoseAICharacter>(InPawn);

	if (PossessedCharacter)
	{
		if (BehaviorTree)
		{
			Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		}
		else
			UE_LOG(LogTemp, Fatal, TEXT("Behavior tree blackboard data not found."));

		Blackboard->SetValueAsFloat(PatrolWaitTimeKey, PossessedCharacter->PatrolWaitTime);

		PossessedCharacter->OnActorDetected.AddDynamic(this, &AWaywardRoseAIController::BeginPerceivingActor);
		PossessedCharacter->OnActorExitDetection.AddDynamic(this, &AWaywardRoseAIController::ProcessTargetLost);
		
		PossessedCharacter->EnterPatrolState();
	}
	else
		UE_LOG(LogTemp, Error, TEXT("WaywardRoseAIController possessed a pawn that is not a subtype of AWaywardRoseAICharacter."))
}

void AWaywardRoseAIController::BeginPerceivingActor(AActor* actor, const float strength)
{
	if (actor->ActorHasTag(TEXT("Player")))
	{
		DetectedActor = actor;
		CurrentPerceptionStrength = strength;

		switch (DetectionState)
		{
		case EDetectionState::LosingFromDetected:
			DetectionState = EDetectionState::Detected;
			break;
		case EDetectionState::Searching:
			TransitionFromSearchState();
			TransitionToDetectedState();
			break;
		case EDetectionState::None:
			TransitionToDetectingState();
			break;
		case EDetectionState::LosingFromDetecting:
			DetectionState = EDetectionState::Detecting;
		default:
			break;
		}
	}
}

void AWaywardRoseAIController::ForceLoseTarget()
{


	switch (DetectionState)
	{
	case EDetectionState::LosingFromDetected:
	case EDetectionState::Detected:
		TransitionFromDetectedState();
		TransitionToSearchState();
		break;
	case EDetectionState::LosingFromDetecting:
	case EDetectionState::Detecting:
		TransitionToNoneState();
	default:
		break;
	}

	DetectedActor = nullptr;
	CurrentPerceptionStrength = 0.0f;
}

void AWaywardRoseAIController::ProcessTargetLost(AActor* actor, const float MaxAge)
{
	CurrentPerceptionStrength = 0.0f;

	if (actor->ActorHasTag(TEXT("Player")))
	{
		switch (DetectionState)
		{
		case EDetectionState::Detected:
			DetectionState = EDetectionState::LosingFromDetected;
			UndetectTimer = 0.0f;
			break;
		case EDetectionState::Detecting:
			DetectionState = EDetectionState::LosingFromDetecting;
			UndetectTimer = 0.0f;
			break;
		default:
			break;
		}

	}
}

void AWaywardRoseAIController::TransitionToSearchState()
{
	GetBlackboardComponent()->SetValueAsVector(TargetLocationKey, DetectedActor->GetActorLocation());
	DetectionState = EDetectionState::Searching;
	SearchTimer = 0.0f;
}

void AWaywardRoseAIController::TransitionToDetectedState()
{
	Blackboard->SetValueAsObject(TargetActorKey, DetectedActor);
	PossessedCharacter->EnterChaseState();
	DetectionState = EDetectionState::Detected;
}

void AWaywardRoseAIController::TransitionToDetectingState()
{
	DetectionTimer = 0.0f;
	PossessedCharacter->EnterDetectingState();
	GetBlackboardComponent()->SetValueAsBool(DetectingTargetKey, true);
	DetectionState = EDetectionState::Detecting;
}

void AWaywardRoseAIController::TransitionToNoneState()
{
	DetectionState = EDetectionState::None;
}

void AWaywardRoseAIController::TransitionFromSearchState()
{
	Blackboard->ClearValue(TargetLocationKey);
}

void AWaywardRoseAIController::TransitionFromDetectedState()
{
	GetBlackboardComponent()->ClearValue(TargetActorKey);
}

void AWaywardRoseAIController::TransitionFromDetectingState()
{
	GetBlackboardComponent()->SetValueAsBool(DetectingTargetKey, false);
	PossessedCharacter->ExitDetectingState();
}
