// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseStateEnemy/NPCBaseEnemyController.h"

#include "AI/Interfaces/AITargetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"


ANPCBaseEnemyController::ANPCBaseEnemyController()
{
	MyControlledPawn = Cast<ANPCBaseEnemy>(GetControlledPawn());
}

void ANPCBaseEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPCBaseEnemyController::InitializeBlackboardValues()
{
	Super::InitializeBlackboardValues();
	if (!Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("Blackboard is not initialized."));
		return;
	}
	
	Blackboard->SetValueAsVector(TEXT("TargetLocation"), FVector::ZeroVector);
	Blackboard->SetValueAsObject(TEXT("AttackTarget"), nullptr);
	Blackboard->SetValueAsFloat(TEXT("TimeBeforeInvestigating"), MyControlledPawn->GetTimeBeforeInvestigating());
	Blackboard->SetValueAsFloat(TEXT("RandomInvestigatingTimeDeviation"), MyControlledPawn->GetRandomInvestigatingTimeDeviation());

	LGDebug::Log("InitializeBlackboardValues",true);
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
		MyControlledPawn->SetEnemyState(EEnemyState::Passive);
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
		MyControlledPawn->SetEnemyState(EEnemyState::Patrolling);
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
		MyControlledPawn->SetEnemyState(EEnemyState::Attacking);
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
		MyControlledPawn->SetEnemyState(EEnemyState::Investigating);
	}
}

void ANPCBaseEnemyController::HandleSight(AActor* Actor, FAIStimulus Stimulus)
{
	
		if (Stimulus.WasSuccessfullySensed())
		{
			if (Stimulus.Type != UAISense::GetSenseID<UAISense_Sight>())return;
			if (MyControlledPawn->GetState() == EEnemyState::Attacking)return;

			if (Actor->Implements<UAITargetInterface>())
			{
				SetStateAsAttacking(Actor);
				LGDebug::Log("SEE PLAYER ",true);
			}
		}
		else
		{
			if (MyControlledPawn->GetState() == EEnemyState::Investigating)return;
			
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
			if (MyControlledPawn->GetState() == EEnemyState::Attacking)return;

			///new restart behaviour i prefer use a jolly state -> SetStateAsPassive
			// if (ControlledPawn->GetState() == EEnemyState::Investigating)
			// {
			// 	if (GetAIOwner())
			// 	{
			// 		GetAIOwner()->StopLogic("Restarting Behavior Tree");
			// 		GetAIOwner()->RunBehaviorTree(BehaviorTreeAsset);  
			// 	}
			// }

			if (GetBlackboardComponent()->GetValueAsBool("HasBeenInvestigating") == false)
			{
				GetBlackboardComponent()->SetValueAsVector("InitialPosition",ControlledPawn->GetActorLocation());
				GetBlackboardComponent()->SetValueAsBool("HasBeenInvestigating",true);
			}
			
			GetBlackboardComponent()->SetValueAsVector("TargetLocation",RandomPosition(Stimulus.StimulusLocation));
			
			SetStateAsPassive();
			SetStateAsInvestigating();
			LGDebug::Log("HEAR PLAYER ",true);
		}
}

void ANPCBaseEnemyController::HandleDamage(AActor* Actor, FAIStimulus Stimulus)
{
	
}

FVector ANPCBaseEnemyController::RandomPosition(FVector Position)
{
	const float MinRadius = MyControlledPawn->GetMinInvestigatingRadius();
	const float MaxRadius = MyControlledPawn->GetMaxInvestigatingRadius();
	
	float RandomRadius = FMath::FRandRange(MinRadius, MaxRadius);
	
	float RandomAngle = FMath::FRandRange(0.0f, 2.0f * PI);
	
	float OffsetX = RandomRadius * FMath::Cos(RandomAngle);
	float OffsetY = RandomRadius * FMath::Sin(RandomAngle);
	
	FVector RandomPoint = Position + FVector(OffsetX, OffsetY, 0.0f);

	return RandomPoint;
}


void ANPCBaseEnemyController::OnLostSight()
{
	Super::OnLostSight();
	
	SetStateAsPatrolling();
	LGDebug::Log("LOST SIGHT PLAYER", true);
}

void ANPCBaseEnemyController::OnLostHear()
{
	Super::OnLostHear();
	
	SetStateAsPatrolling();
	LGDebug::Log(" LOST HEAR PLAYER ",true);
}

void ANPCBaseEnemyController::OnLostDamage()
{
	Super::OnLostDamage();
	
	SetStateAsPatrolling();
	LGDebug::Log(" LOST DAMAGE PLAYER ",true);
}

