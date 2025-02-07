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
		if (UBehaviorTree* const Tree = EnemyBase->GetBehaviorTree())
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(Tree->BlackboardAsset,BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(Tree);
			//LGDebug::Log("aic controller inizializzata",true);
		}
	}
}

void ANPCBaseController::BeginPlay()
{
	Super::BeginPlay();
	InitializeBlackboardValues();

	if (auto PhatFollowComp = FindComponentByClass<UEnemyCrowdFollowingComponent>())
	{
		SetPathFollowingComponent(PhatFollowComp);
		if (UEnemyCrowdFollowingComponent* EnemyCrowdFollowing = Cast<UEnemyCrowdFollowingComponent>(PhatFollowComp))
		{
			EnemyCrowdFollowing->Initialize();
			EnemyCrowdFollowing->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
			EnemyCrowdFollowing->SetCrowdCollisionQueryRange(1000.0f);
			EnemyCrowdFollowing->SetCrowdSeparationWeight(.5f);
			EnemyCrowdFollowing->UpdateCrowdAgentParams();
			
			LGDebug::Log("aic controller inizializzata",true);
		}
	}
	
}
