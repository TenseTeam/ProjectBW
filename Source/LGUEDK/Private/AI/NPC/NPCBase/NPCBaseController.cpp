// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBase/NPCBaseController.h"

#include "AI/NPC/NPCBase/NPCBase.h"
#include "Components/AICrowdFollowingComponent/EnemyCrowdFollowingComponent.h"
#include "Utility/LGDebug.h"


ANPCBaseController::ANPCBaseController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UEnemyCrowdFollowingComponent>(TEXT("PhatFollowingComponent")))
{
	ControlledPawn = nullptr;
	
	
}

void ANPCBaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPCBase* const EnemyBase = Cast<ANPCBase>(InPawn))
	{
		ControlledPawn = EnemyBase;
		if (UBehaviorTree* const tree = EnemyBase->GetBehaviorTree())
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(tree->BlackboardAsset,BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(tree);
			//LGDebug::Log("aic controller inizializzata",true);
		}
	}
}

void ANPCBaseController::BeginPlay()
{
	Super::BeginPlay();
	InitializeBlackboardValues();

	auto PhatFollowComp = FindComponentByClass<UEnemyCrowdFollowingComponent>();
	if (PhatFollowComp)
	{
		SetPathFollowingComponent(PhatFollowComp);
		UEnemyCrowdFollowingComponent* EnemyCrowdFollowing = Cast<UEnemyCrowdFollowingComponent>(PhatFollowComp);
		if (EnemyCrowdFollowing)
		{
			EnemyCrowdFollowing->Initialize();
			EnemyCrowdFollowing->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
			EnemyCrowdFollowing->SetCrowdCollisionQueryRange(5000.0f);
			EnemyCrowdFollowing->SetCrowdSeparationWeight(2000.0f);
			EnemyCrowdFollowing->SetCrowdSeparation(true);
			EnemyCrowdFollowing->SetCrowdAnticipateTurns(true);
			EnemyCrowdFollowing->UpdateCrowdAgentParams();
			
			LGDebug::Log("aic controller inizializzata",true);
		}
	}
	
}
