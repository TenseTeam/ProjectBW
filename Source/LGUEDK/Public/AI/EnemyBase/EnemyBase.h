// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyControllerBase.h" 
#include "EnemyBase.generated.h"

UCLASS()
class LGUEDK_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UBehaviorTree* GetBehaviorTree() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "IA",meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree;
	
};
