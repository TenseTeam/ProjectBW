// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBaseEnemy.h"
#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NPCPerceptionSystemController.generated.h"

class ANPCBaseEnemy;


UCLASS()
class LGUEDK_API ANPCPerceptionSystemController : public ANPCBaseController
{
	GENERATED_BODY()

public:
	
	ANPCPerceptionSystemController();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sense")
	bool CanSee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sense")
	bool CanHear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sense")
	bool CanTakeDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee"))
	float SightRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee"))
	float LoseSightRadius = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "180.0", UIMax = "180.0", EditCondition = "CanSee"))
	float PeripheralVisionAngleDegrees = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee"))
	float AutoSuccessRangeFromLastSeenLocation = 520.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee"))
	float SightMaxAge = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanHear"))
	float HearingRange = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanHear"))
	float HearingMaxAge = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanTakeDamage"))
	float DamageMaxAge = 5.0f;

	
	FTimerHandle LostSightTimerHandle;
	
	FTimerHandle LostHearTimerHandle;

	FTimerHandle LostDamageTimerHandle;


	virtual void BeginPlay() override;
	
	virtual void InitializeBlackboardValues() override {};
	
	UFUNCTION()
	virtual void HandleSight(AActor* Actor, FAIStimulus Stimulus) {};
	UFUNCTION()
	virtual void HandleHear(AActor* Actor, FAIStimulus Stimulus) {};
	UFUNCTION()
	virtual void HandleDamage(AActor* Actor, FAIStimulus Stimulus) {};
	
	UFUNCTION()
	virtual void OnLostSight() {};
	UFUNCTION()
	virtual void OnLostHear() {};
	UFUNCTION()
	virtual void OnLostDamage() {};
	
private:

	UPROPERTY()
	UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY()
	UAISenseConfig_Damage* DamageConfig;
	
	void SetUpPerceptionSystem();
	void SetUpSightConfig();
	void SetUpHearingConfig();
	void SetUpDamageConfig();
	
};


