// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyControllerBase.h" 
#include "AI/Enumerators/EMovementSpeed.h"
#include "AI/Enumerators/EnemyState.h"
#include "EnemyBase.generated.h"

UCLASS()
class LGUEDK_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
	float IdleSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
	float WalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
	float RunSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|State")
	EEnemyState CurrentState = EEnemyState::Patrolling;
	
public:
	// Sets default values for this character's properties
	AEnemyBase();

	UBehaviorTree* GetBehaviorTree() const;

	void SetMovementSpeed(EMovementSpeed MovementSpeed);

	EEnemyState GetEnemyState() const { return CurrentState; }
	void SetEnemyState(EEnemyState NewState) { CurrentState = NewState; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "IA",meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree;
	
};
