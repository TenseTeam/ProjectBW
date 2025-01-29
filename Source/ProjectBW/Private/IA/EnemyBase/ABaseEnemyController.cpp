// Copyright Villains, Inc. All Rights Reserved.


#include "IA/EnemyBase/ABaseEnemyController.h"

#include <string>

#include "AI/Interfaces/AITargetInterface.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HealtComponent/HealthBaseComponent.h"


ABaseEnemyController::ABaseEnemyController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EnemyBase = nullptr;
}

void ABaseEnemyController::BeginPlay()
{
	EnemyBase = Cast<AEnemyBase>(GetControlledPawn());
	if (!EnemyBase)
	{
		LGDebug::Log("EnemyBase NON INIZIALIZZATA",true);
		return;
	}
	Super::BeginPlay();
	
}

void ABaseEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABaseEnemyController::InitializeBlackboardValues()
{
	Super::InitializeBlackboardValues();
	if (!Blackboard)
	{
		LGDebug::Log("blackboard NON INIZIALIZZATA",true);
		return;
	}
	
	Blackboard->SetValueAsVector("InitialPosition",EnemyBase->GetActorLocation());
	Blackboard->SetValueAsFloat(TEXT("SearchRadius"), EnemyBase->GetSearchRadius());
	Blackboard->SetValueAsFloat(TEXT("TimeBeforeNextStep"), EnemyBase->GetTimeBeforeNextStep());
	Blackboard->SetValueAsVector(TEXT("TargetLocation"), FVector::ZeroVector);
	Blackboard->SetValueAsObject(TEXT("AttackTarget"), nullptr);
	Blackboard->SetValueAsFloat(TEXT("TimeBeforeInvestigating"), EnemyBase->GetTimeBeforeInvestigating());
	Blackboard->SetValueAsFloat(TEXT("RandomInvestigatingTimeDeviation"), EnemyBase->GetRandomInvestigatingTimeDeviation());
	Blackboard->SetValueAsFloat(TEXT("MaxAttackRadius"), EnemyBase->GetMaxRadius());
	Blackboard->SetValueAsFloat(TEXT("MinAttackRadius"), EnemyBase->GetMinRadius());
	Blackboard->SetValueAsFloat(TEXT("StrafeRadius"), EnemyBase->GetStrafeRadius());
	Blackboard->SetValueAsFloat(TEXT("JumpHeight"), EnemyBase->GetJumpHeight());
	Blackboard->SetValueAsFloat(TEXT("JumpDistance"), EnemyBase->GetJumpDistance());
	
	// LGDebug::Log("InitializeBlackboardValues",true);
	// LGDebug::Log(FString::SanitizeFloat(EnemyBase->GetRandomRadius()), true);
	// LGDebug::Log(FString::SanitizeFloat(EnemyBase->GetMinRadius()), true);
}

void ABaseEnemyController::SetStateAsPassive()
{
	Super::SetStateAsPassive();
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Passive));

		//LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Passive).ToString(),true);
	}

	if (EnemyBase)
	{
		EnemyBase->OnEnemyPassive();
		//LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Passive).ToString(),true);
		
	}
}

void ABaseEnemyController::SetStateAsPatrolling()
{
	Super::SetStateAsPatrolling();
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8 (EEnemyState::Patrolling));

		//LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Patrolling).ToString(),true);
	}

	if (EnemyBase)
	{
		EnemyBase->OnEnemyPatrolling();
	}
}

void ABaseEnemyController::SetStateAsAttacking(AActor* Actor)
{
	Super::SetStateAsAttacking(Actor);
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Attacking));
		BlackboardComp->SetValueAsObject(TEXT("AttackTarget"),Actor);
		//LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Attacking).ToString(),true);
	}
	
	if (EnemyBase)
	{
		EnemyBase->OnEnemyAttack(Actor);
	}
	
}

void ABaseEnemyController::SetStateAsInvestigating()
{
	Super::SetStateAsInvestigating();
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Investigating));
	}

	if (EnemyBase)
	{
		EnemyBase->OnEnemyInvestigating();
	}
}

void ABaseEnemyController::SetStateAsDead()
{
	Super::SetStateAsDead();
	
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Dead));
		LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Dead).ToString(),true);
	}
	if (EnemyBase)
	{
		EnemyBase->SetEnemyState(EEnemyState::Dead);
		LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Dead).ToString(),true);
	}
}


void ABaseEnemyController::HandleSight(AActor* Actor, FAIStimulus Stimulus)
{
	Super::HandleSight(Actor, Stimulus);
	int SightActorTeamIndex;
	int MyIndex;
	
	if (Stimulus.WasSuccessfullySensed())
	{
		if (Stimulus.Type != UAISense::GetSenseID<UAISense_Sight>())return;
		
		SightActorTeamIndex = GetTeamIndex(Actor);
		MyIndex = GetTeamIndex(EnemyBase);
			
		if (GetWorld()->GetTimerManager().IsTimerActive(LostSightTimerHandle) && SightActorTeamIndex != MyIndex)
		{
			GetWorld()->GetTimerManager().ClearTimer(LostSightTimerHandle);
			//LGDebug::Log("TIMER PERSO ANNULLATO", true);
		}

		if (SightActorTeamIndex == MyIndex)return;
		if (EnemyBase->GetState() == EEnemyState::Dead)return;
		if (EnemyBase->GetState() == EEnemyState::Attacking)return;
		
		SetStateAsAttacking(Actor);
		LGDebug::Log("SEE PLAYER ",true);
		
	}
	else
	{
		if (EnemyBase->GetState() == EEnemyState::Dead)return;
		if (EnemyBase->GetState() == EEnemyState::Investigating)return;
		if (!Actor->Implements<UAITargetInterface>()) return;
		
		SightActorTeamIndex = GetTeamIndex(Actor);
		MyIndex = GetTeamIndex(EnemyBase);
		
		if (SightActorTeamIndex == MyIndex)return;
		
		GetWorld()->GetTimerManager().SetTimer(
			LostSightTimerHandle,
			this,
			&ABaseEnemyController::OnLostSight,
			SightMaxAge,
			false
		);
		//LGDebug::Log("LOST SIGHT PLAYER", true);
		
		// ClearFocus(EAIFocusPriority::Gameplay);
	}		
	
}

void ABaseEnemyController::HandleHear(AActor* Actor, FAIStimulus Stimulus)
{
	Super::HandleHear(Actor, Stimulus);
	if (Stimulus.WasSuccessfullySensed())
	{
		if (Stimulus.Type != UAISense::GetSenseID<UAISense_Hearing>())return;
		if (EnemyBase->GetState() == EEnemyState::Attacking)return;

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
			GetBlackboardComponent()->SetValueAsBool("HasBeenInvestigating",true);
		}
			
		GetBlackboardComponent()->SetValueAsVector("TargetLocation",EnemyBase->RandomPosition(Stimulus.StimulusLocation));
			
		SetStateAsPassive();
		SetStateAsInvestigating();
		LGDebug::Log("HEAR PLAYER ",true);
	}
}

void ABaseEnemyController::HandleDamage(AActor* Actor, FAIStimulus Stimulus)
{
	Super::HandleDamage(Actor, Stimulus);
	if (Stimulus.WasSuccessfullySensed())
	{
		if (Stimulus.Type != UAISense::GetSenseID<UAISense_Damage>())return;
		if (EnemyBase->GetState() == EEnemyState::Dead)return;
		if (EnemyBase->GetState() == EEnemyState::Attacking)return;
		SetStateAsAttacking(Actor);
	}
}


void ABaseEnemyController::OnLostSight()
{
	Super::OnLostSight();
	SetStateAsPatrolling();
	
	LGDebug::Log("LOST SIGHT PLAYER", true);
}

void ABaseEnemyController::OnLostHear()
{
	Super::OnLostHear();
	
	SetStateAsPatrolling();
	LGDebug::Log(" LOST HEAR PLAYER ",true);
}

void ABaseEnemyController::OnLostDamage()
{
	Super::OnLostDamage();
	
	SetStateAsPatrolling();
	LGDebug::Log(" LOST DAMAGE PLAYER ",true);
}

int ABaseEnemyController::GetTeamIndex(AActor* Actor)
{
	if (Actor->Implements<UAITargetInterface>())
	{
		int index = IAITargetInterface::Execute_GetTeamIndex(Actor);
		return  index;
	}
	
	LGDebug::Log(" NON IMPLEMENTA INTERFACCIA " + Actor->GetName(),true);
	return -1;
	
}




