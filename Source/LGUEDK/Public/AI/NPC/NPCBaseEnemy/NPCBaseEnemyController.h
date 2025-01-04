// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBaseEnemy.h"
#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NPCBaseEnemyController.generated.h"

class ANPCBaseEnemy;


UCLASS()
class LGUEDK_API ANPCBaseEnemyController : public ANPCBaseController
{
	GENERATED_BODY()

public:

	explicit ANPCBaseEnemyController(FObjectInitializer const& ObjectInitializer);

protected:
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void SetStateAsPatrolling();
	
	UFUNCTION()
	void SetStateAsPassive();

	UFUNCTION()
	void SetStateAsAttacking(AActor* Actor);

	UFUNCTION()
	void SetStateAsInvestigating();

	UFUNCTION()
	void SetStateAsJumping(AActor* Actor);


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SightRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float LoseSightRadius = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "180.0", UIMax = "180.0"))
	float PeripheralVisionAngleDegrees = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AutoSuccessRangeFromLastSeenLocation = 520.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SightMaxAge = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float HearingRange = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float HearingMaxAge = 5.0f;

	
private:
	
	UPROPERTY()
	ANPCBaseEnemy* ControlledPawn;

	UPROPERTY()
	UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;
	
	void SetUpPerceptionSystem();
	void SetUpSightConfig();
	void SetUpHearingConfig();
	

	UFUNCTION()
	void HandleSight(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void HandleHear(AActor* Actor, FAIStimulus Stimulus);
	
	
	UFUNCTION()
	void OnLostSight();
	UFUNCTION()
	void OnLostHear();

	
	FTimerHandle LostSightTimerHandle;
	
	FTimerHandle LostHearTimerHandle;
	
	
};
