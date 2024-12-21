// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "EnemyControllerBase.generated.h"



UCLASS()
class LGUEDK_API AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()

public:

	explicit AEnemyControllerBase(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnPossess(APawn* InPawn) override;

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
	
private:
	class UAISenseConfig_Sight* SightConfig;
	
	void SetUpPercveptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor,FAIStimulus Stimulus);
};
