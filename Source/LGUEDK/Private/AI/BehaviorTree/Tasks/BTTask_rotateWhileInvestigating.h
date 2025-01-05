// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_rotateWhileInvestigating.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_rotateWhileInvestigating : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	explicit UBTTask_rotateWhileInvestigating(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void UpdateRotation(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime);

public:
	UPROPERTY(EditAnywhere, Category = "Investigate")
	float RotationInterval = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Investigate")
	float MaxRotationAngle = 45.0f;

	UPROPERTY(EditAnywhere, Category = "Investigate")
	float RotationSpeed = 5;
	
	FTimerHandle RotationTimer;
	
	int32 CurrentDirection = 1;
	
	APawn* ControlledPawn = nullptr;
};
