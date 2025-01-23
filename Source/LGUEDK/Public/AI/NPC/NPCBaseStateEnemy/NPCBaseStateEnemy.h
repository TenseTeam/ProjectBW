// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/EQS/EQS_Manager.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "NPCBaseStateEnemy.generated.h"

UCLASS()
class LGUEDK_API ANPCBaseStateEnemy : public ANPCBaseEnemy
{
	GENERATED_BODY()

public:

	ANPCBaseStateEnemy();
		
	UFUNCTION(BlueprintCallable)
	FVector RandomPosition(FVector Position);

	UFUNCTION()
	virtual float GetSearchRadius() const { return SearchRadius; }
	UFUNCTION()
	virtual float GetTimeBeforeNextStep() const { return TimeBeforeNextStep; }
	UFUNCTION()
	virtual bool GetWantExplore() const{return bWantExplore;}
	UFUNCTION()
	virtual int GetMinInvestigatingRadius() const { return MinInvestigatingRadius; }
	UFUNCTION()
	virtual int GetMaxInvestigatingRadius() const { return MaxInvestigatingRadius; }
	UFUNCTION()
	virtual float GetTimeBeforeInvestigating() const { return TimeBeforeInvestigating; }
	UFUNCTION()
	virtual float GetRandomInvestigatingTimeDeviation() const { return RandomInvestigatingTimeDeviation; }
	
	UFUNCTION()
	virtual float GetMinRadius() const {return MinAttackRadius; }
	UFUNCTION()
	virtual float GetMaxRadius() {return MaxAttackRadius; }
	UFUNCTION()
	virtual float GetStrafeRadius() {return MaxStrafeRadius; }
	
	UFUNCTION()
	virtual float GetJumpHeight() const {return MaxJumpingHeight; }
	UFUNCTION()
	virtual float GetJumpDistance() const {return MinJumpingDistance; }
	
	void SetAttackTarget(AActor* Target) { AttackTarget = Target; }
	UFUNCTION(BlueprintPure)
	AActor* GetAttackTarget() const { return AttackTarget; }
	
	UPROPERTY(VisibleAnywhere, Category = "AI|EQS")
	AEQS_Manager* EQS_Manager;
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrolling")
	float SearchRadius = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrolling", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
	float TimeBeforeNextStep = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrolling")
	bool bWantExplore;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000"))
	float MinInvestigatingRadius = 200;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "2000", UIMin = "0", UIMax = "2000"))
	float MaxInvestigatingRadius = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
	float TimeBeforeInvestigating = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
	float RandomInvestigatingTimeDeviation = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Jumping")
	float MaxJumpingHeight = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Jumping")
	float MinJumpingDistance = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack Ranges")
	float MinAttackRadius = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack Ranges")
	float MaxAttackRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack Ranges")
	float MaxStrafeRadius = 1000.f;
	
	UPROPERTY()
	AActor* AttackTarget;
	
};
