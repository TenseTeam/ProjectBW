// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "BWNPCBaseEnemy.generated.h"

UCLASS()
class LGUEDK_API ABWNPCBaseEnemy : public ANPCBaseEnemy
{
	GENERATED_BODY()

public:

	ABWNPCBaseEnemy();
		
	UFUNCTION(BlueprintCallable)
	FVector RandomPosition(FVector Position);
	
	int GetMinInvestigatingRadius() const { return MinInvestigatingRadius; }
	int GetMaxInvestigatingRadius() const { return MaxInvestigatingRadius; }
	float GetTimeBeforeInvestigating() const { return TimeBeforeInvestigating; }
	float GetRandomInvestigatingTimeDeviation() const { return RandomInvestigatingTimeDeviation; }
	UFUNCTION()
	virtual float GetMinRadius() const {return 0; }
	UFUNCTION()
	virtual float GetRandomRadius() {return 0; }
	UFUNCTION()
	virtual float GetRandomStrafeRadius() {return 0; }
	UFUNCTION()
	float GetJumpHeight() const {return MaxJumpingHeight; }
	
	void SetAttackTarget(AActor* Target) { AttackTarget = Target; }
	UFUNCTION(BlueprintPure)
	AActor* GetAttackTarget() const { return AttackTarget; }
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000"))
	int MinInvestigatingRadius = 200;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "2000", UIMin = "0", UIMax = "2000"))
	int MaxInvestigatingRadius = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
	float TimeBeforeInvestigating = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
	float RandomInvestigatingTimeDeviation = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Jumping", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
	float MaxJumpingHeight = 500.f;
	
	UPROPERTY()
	AActor* AttackTarget;
	
};
