// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemyController.h"

#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"


ANPCBaseEnemyController::ANPCBaseEnemyController(FObjectInitializer const& ObjectInitializer)
{
	SetUpPercveptionSystem();
}

void ANPCBaseEnemyController::BeginPlay()
{
	Super::BeginPlay();
	SetStateAsPatrolling();
}

void ANPCBaseEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPCBaseEnemy* const EnemyBase = Cast<ANPCBaseEnemy>(InPawn))
	{
		ControlledPawn = EnemyBase;
		if (UBehaviorTree* const tree = EnemyBase->GetBehaviorTree())
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(tree->BlackboardAsset,BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(tree);
			UE_LOG(LogTemp, Warning, TEXT("Aic controller: %s"), *InPawn->GetName());
		}
	}
}

void ANPCBaseEnemyController::SetStateAsPatrolling()
{
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8 (EEnemyState::Patrolling));
	}

	if (ControlledPawn)
	{
		ControlledPawn->SetEnemyState(EEnemyState::Patrolling);
	}
}

void ANPCBaseEnemyController::SetStateAsPassive()
{
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Passive));
	}

	if (ControlledPawn)
	{
		ControlledPawn->SetEnemyState(EEnemyState::Passive);
	}
}

void ANPCBaseEnemyController::SetUpPercveptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = SightRadius;
		SightConfig->LoseSightRadius = LoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = AutoSuccessRangeFromLastSeenLocation;
		SightConfig->SetMaxAge(SightMaxAge);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&ANPCBaseEnemyController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void ANPCBaseEnemyController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	// if (IAITargetInterface* MyInterface = Cast<IAITargetInterface>(Actor))
	// {
	// 	GetBlackboardComponent()->SetValueAsBool("CanSeePlayer",Stimulus.WasSuccessfullySensed());
	// 	UE_LOG(LogTemp, Warning, TEXT("Vedo il player"),Stimulus.WasSuccessfullySensed());
	// }
	if (AActor* const player = Actor)
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer",Stimulus.WasSuccessfullySensed());
		UE_LOG(LogTemp, Warning, TEXT("Vedo il player"),Stimulus.WasSuccessfullySensed());
	}
}
