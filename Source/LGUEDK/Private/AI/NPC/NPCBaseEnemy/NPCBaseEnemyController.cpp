// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemyController.h"

#include "AI/Interfaces/AITargetInterface.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Hearing.h"
#include "Utility/LGDebug.h"


ANPCBaseEnemyController::ANPCBaseEnemyController(FObjectInitializer const& ObjectInitializer)
{
	SetUpPerceptionSystem();
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

		LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Patrolling).ToString(),true);
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

		LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Passive).ToString(),true);
	}

	if (ControlledPawn)
	{
		ControlledPawn->SetEnemyState(EEnemyState::Passive);
	}
}

void ANPCBaseEnemyController::SetStateAsAttacking(AActor* Actor)
{
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Attacking));
		BlackboardComp->SetValueAsObject(TEXT("AttackTarget"),Actor);

		LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Attacking).ToString(),true);
	}

	if (ControlledPawn)
	{
		ControlledPawn->SetEnemyState(EEnemyState::Attacking);
	}
}

void ANPCBaseEnemyController::SetStateAsInvestigating()
{
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Investigating));
	}

	if (ControlledPawn)
	{
		ControlledPawn->SetEnemyState(EEnemyState::Investigating);
	}
}

void ANPCBaseEnemyController::SetStateAsJumping(AActor* Actor)
{
	
}

void ANPCBaseEnemyController::SetUpPerceptionSystem()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	SetUpSightConfig();
	SetUpHearingConfig();
}

void ANPCBaseEnemyController::SetUpSightConfig()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SightConfig->SightRadius = SightRadius;
		SightConfig->LoseSightRadius = LoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = AutoSuccessRangeFromLastSeenLocation;
		SightConfig->SetMaxAge(SightMaxAge);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCBaseEnemyController::HandleSight);
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
	}
}

void ANPCBaseEnemyController::SetUpHearingConfig()
{
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = HearingRange;
		HearingConfig->SetMaxAge(HearingMaxAge);
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCBaseEnemyController::HandleHear);
		AIPerceptionComponent->ConfigureSense(*HearingConfig);
	}
}

void ANPCBaseEnemyController::HandleSight(AActor* Actor, FAIStimulus Stimulus)
{
	
		if (Stimulus.WasSuccessfullySensed())
		{
			if (Stimulus.Type != UAISense::GetSenseID<UAISense_Sight>())return;
			if (ControlledPawn->GetState() == EEnemyState::Attacking)return;

			if (Actor->Implements<UAITargetInterface>())
			{
				SetStateAsAttacking(Actor);
				LGDebug::Log("SEE PLAYER ",true);
			}
		}
		else
		{
			if (ControlledPawn->GetState() == EEnemyState::Investigating)return;
			
			GetWorld()->GetTimerManager().SetTimer(
				LostSightTimerHandle,
				this,
				&ANPCBaseEnemyController::OnLostSight,
				SightMaxAge,
				false
			);
			LGDebug::Log("LOST SIGHT PLAYER", true);
		}	
	
}

void ANPCBaseEnemyController::HandleHear(AActor* Actor, FAIStimulus Stimulus)
{

		if (Stimulus.WasSuccessfullySensed())
		{
			if (Stimulus.Type != UAISense::GetSenseID<UAISense_Hearing>())return;
			if (ControlledPawn->GetState() == EEnemyState::Attacking)return;

			///new restart behaviour i prefer use a jolly state -> SetStateAsPassive
			// if (ControlledPawn->GetState() == EEnemyState::Investigating)
			// {
			// 	if (GetAIOwner())
			// 	{
			// 		GetAIOwner()->StopLogic("Restarting Behavior Tree");
			// 		GetAIOwner()->RunBehaviorTree(BehaviorTreeAsset);  
			// 	}
			// }
			
			GetBlackboardComponent()->SetValueAsVector("TargetLocation",RandomPosition(Stimulus.StimulusLocation));
			SetStateAsPassive();
			SetStateAsInvestigating();
			LGDebug::Log("HEAR PLAYER ",true);
		}
}

FVector ANPCBaseEnemyController::RandomPosition(FVector Position)
{
	const float MinRadius = 200.0f;
	const float MaxRadius = 600.0f;
	
	float RandomRadius = FMath::FRandRange(MinRadius, MaxRadius);
	
	float RandomAngle = FMath::FRandRange(0.0f, 2.0f * PI);
	
	float OffsetX = RandomRadius * FMath::Cos(RandomAngle);
	float OffsetY = RandomRadius * FMath::Sin(RandomAngle);
	
	FVector RandomPoint = Position + FVector(OffsetX, OffsetY, 0.0f);

	return RandomPoint;
}


void ANPCBaseEnemyController::OnLostSight()
{
	SetStateAsPatrolling();
	LGDebug::Log("LOST SIGHT PLAYER", true);
}

void ANPCBaseEnemyController::OnLostHear()
{
	SetStateAsPatrolling();
	LGDebug::Log(" LOST HEAR PLAYER ",true);
}
