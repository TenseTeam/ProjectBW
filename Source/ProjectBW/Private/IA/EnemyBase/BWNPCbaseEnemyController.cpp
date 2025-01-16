// Copyright Villains, Inc. All Rights Reserved.


#include "IA/EnemyBase/BWNPCbaseEnemyController.h"

#include <string>

#include "AI/Interfaces/AITargetInterface.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HealtComponent/HealthBaseComponent.h"


ABWNPCbaseEnemyController::ABWNPCbaseEnemyController()
{
	BWControlledPawn = nullptr;
}

void ABWNPCbaseEnemyController::BeginPlay()
{
	BWControlledPawn = Cast<ANPCBaseStateEnemy>(GetControlledPawn());
	Super::BeginPlay();
	
}

void ABWNPCbaseEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABWNPCbaseEnemyController::InitializeBlackboardValues()
{
	Super::InitializeBlackboardValues();
	if (!Blackboard)
	{
		LGDebug::Log("blackboard NON INIZIALIZZATA",true);
		return;
	}
	
	Blackboard->SetValueAsVector(TEXT("TargetLocation"), FVector::ZeroVector);
	Blackboard->SetValueAsObject(TEXT("AttackTarget"), nullptr);
	Blackboard->SetValueAsFloat(TEXT("TimeBeforeInvestigating"), BWControlledPawn->GetTimeBeforeInvestigating());
	Blackboard->SetValueAsFloat(TEXT("RandomInvestigatingTimeDeviation"), BWControlledPawn->GetRandomInvestigatingTimeDeviation());
	Blackboard->SetValueAsFloat(TEXT("MaxAttackRadius"), BWControlledPawn->GetMaxRadius());
	Blackboard->SetValueAsFloat(TEXT("MinAttackRadius"), BWControlledPawn->GetMinRadius());
	Blackboard->SetValueAsFloat(TEXT("StrafeRadius"), BWControlledPawn->GetRandomStrafeRadius());
	Blackboard->SetValueAsFloat(TEXT("JumpHeight"), BWControlledPawn->GetJumpHeight());
	Blackboard->SetValueAsFloat(TEXT("JumpDistance"), BWControlledPawn->GetJumpDistance());
	
	// LGDebug::Log("InitializeBlackboardValues",true);
	// LGDebug::Log(FString::SanitizeFloat(BWControlledPawn->GetRandomRadius()), true);
	// LGDebug::Log(FString::SanitizeFloat(BWControlledPawn->GetMinRadius()), true);
}

void ABWNPCbaseEnemyController::SetStateAsPassive()
{
	Super::SetStateAsPassive();
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Passive));

		LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Passive).ToString(),true);
	}

	if (BWControlledPawn)
	{
		BWControlledPawn->SetEnemyState(EEnemyState::Passive);
	}
}

void ABWNPCbaseEnemyController::SetStateAsPatrolling()
{
	Super::SetStateAsPatrolling();
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8 (EEnemyState::Patrolling));

		LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Patrolling).ToString(),true);
	}

	if (BWControlledPawn)
	{
		BWControlledPawn->SetEnemyState(EEnemyState::Patrolling);
	}
}

void ABWNPCbaseEnemyController::SetStateAsAttacking(AActor* Actor)
{
	Super::SetStateAsAttacking(Actor);
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Attacking));
		BlackboardComp->SetValueAsObject(TEXT("AttackTarget"),Actor);
		LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Attacking).ToString(),true);
	}
	
	if (BWControlledPawn)
	{
		BWControlledPawn->SetEnemyState(EEnemyState::Attacking);
		BWControlledPawn->SetAttackTarget(Actor);
	}
	
}

void ABWNPCbaseEnemyController::SetStateAsInvestigating()
{
	Super::SetStateAsInvestigating();
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Investigating));
	}

	if (BWControlledPawn)
	{
		BWControlledPawn->SetEnemyState(EEnemyState::Investigating);
	}
}

void ABWNPCbaseEnemyController::HandleSight(AActor* Actor, FAIStimulus Stimulus)
{
	Super::HandleSight(Actor, Stimulus);
	LGDebug::Log("VEDO PLAYER ",true);
	if (Stimulus.WasSuccessfullySensed())
	{
		if (Stimulus.Type != UAISense::GetSenseID<UAISense_Sight>())return;
		
		if (Actor->Implements<UAITargetInterface>())
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(LostSightTimerHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(LostSightTimerHandle);
				LGDebug::Log("TIMER PERSO ANNULLATO", true);
			}
			
			SetStateAsAttacking(Actor);
			LGDebug::Log("SEE PLAYER ",true);
		}
	}
	else
	{
		if (BWControlledPawn->GetState() == EEnemyState::Investigating)return;
			
		GetWorld()->GetTimerManager().SetTimer(
			LostSightTimerHandle,
			this,
			&ABWNPCbaseEnemyController::OnLostSight,
			SightMaxAge,
			false
		);
		LGDebug::Log("LOST SIGHT PLAYER", true);
		
		ClearFocus(EAIFocusPriority::Gameplay);
	}		
	
}

void ABWNPCbaseEnemyController::HandleHear(AActor* Actor, FAIStimulus Stimulus)
{
	Super::HandleHear(Actor, Stimulus);
	if (Stimulus.WasSuccessfullySensed())
	{
		if (Stimulus.Type != UAISense::GetSenseID<UAISense_Hearing>())return;
		if (BWControlledPawn->GetState() == EEnemyState::Attacking)return;

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
			
		GetBlackboardComponent()->SetValueAsVector("TargetLocation",BWControlledPawn->RandomPosition(Stimulus.StimulusLocation));
			
		SetStateAsPassive();
		SetStateAsInvestigating();
		LGDebug::Log("HEAR PLAYER ",true);
	}
}

void ABWNPCbaseEnemyController::HandleDamage(AActor* Actor, FAIStimulus Stimulus)
{
	Super::HandleDamage(Actor, Stimulus);
	if (Stimulus.WasSuccessfullySensed())
	{
		if (Stimulus.Type != UAISense::GetSenseID<UAISense_Damage>())return;
		if (BWControlledPawn->GetState() == EEnemyState::Attacking)return;
		SetStateAsAttacking(Actor);
	}
}


void ABWNPCbaseEnemyController::OnLostSight()
{
	Super::OnLostSight();
	SetStateAsPatrolling();
	LGDebug::Log("LOST SIGHT PLAYER", true);
}

void ABWNPCbaseEnemyController::OnLostHear()
{
	Super::OnLostHear();
	
	SetStateAsPatrolling();
	LGDebug::Log(" LOST HEAR PLAYER ",true);
}

void ABWNPCbaseEnemyController::OnLostDamage()
{
	Super::OnLostDamage();
	
	SetStateAsPatrolling();
	LGDebug::Log(" LOST DAMAGE PLAYER ",true);
}




