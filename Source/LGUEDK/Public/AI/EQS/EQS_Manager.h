// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/EEnemyType.h"
#include "GameFramework/Actor.h"
#include "EQS_Manager.generated.h"

UCLASS()
class LGUEDK_API AEQS_Manager : public AActor
{
	GENERATED_BODY()

public:
	
	AEQS_Manager();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "AI|")
	bool CanSearchMeleePoint;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "AI|")
	bool CanSearchRengedPoint;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FVector GetPoint(EEnemyType& EnemyType);


protected:

	virtual void BeginPlay() override;
	
	TMap<EEnemyType,TArray<FVector>>Points;
	
	UPROPERTY(EditAnywhere, Category = " AI|Melee Parameters")
	float MinAttackRadiusMelee = 300.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Melee Parameters")
	float MaxAttackRadiusMelee = 500.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI|Ranged Parameters")
	float MinAttackRadiusRanged = 800.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Ranged Parameters")
	float MaxAttackRadiusRanged = 1200.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI|EQS Parameters")
	float UpdateMeleeInterval = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI|EQS Parameters")
	float UpdateRangedInterval = 5.0f;
	
	void UpdateStrafeMeleePoints();
	void UpdateStrafeRangedPoints();
	
	
private:

	UPROPERTY()
	ACharacter* AttackTarget;
	
	float CurrentMeleeTime = 0;
	float CurrentRangedTime = 0;

	UFUNCTION(BlueprintCallable)
	void Initi();

	UFUNCTION(BlueprintCallable)
	bool IsHittingSomething(const FVector& Start, const FVector& End);

	UFUNCTION(BlueprintCallable)
	bool IsInRange(FVector& TargetPosition,FVector& CurrentPosition, const float& MinDistance, const float& MaxDistance);
};



