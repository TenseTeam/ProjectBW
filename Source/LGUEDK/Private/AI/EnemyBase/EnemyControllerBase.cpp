// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EnemyBase/EnemyControllerBase.h"
#include "AI/EnemyBase/EnemyBase.h"
#include "AI/Interfaces/AITargetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HealtComponent/HealthBaseComponent.h"


AEnemyControllerBase::AEnemyControllerBase(FObjectInitializer const& ObjectInitializer)
{
	SetUpPercveptionSystem();
}

// Called when the game starts or when spawned
void AEnemyControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AEnemyBase* const enemyBase = Cast<AEnemyBase>(InPawn))
	{
		if (UBehaviorTree* const tree = enemyBase->GetBehaviorTree())
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(tree->BlackboardAsset,BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(tree);

			UE_LOG(LogTemp, Warning, TEXT("Aic controller: %s"), *InPawn->GetName());
		}
	}
}

void AEnemyControllerBase::SetUpPercveptionSystem()
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
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&AEnemyControllerBase::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
	
}

void AEnemyControllerBase::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
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


