// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IA/EnemyBase/EnemyBase.h"
#include "MeleeEnemy.generated.h"

UCLASS()
class PROJECTBW_API AMeleeEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMeleeEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
