// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IA/EnemyBase/EnemyBase.h"
#include "RangedEnemy.generated.h"

UCLASS()
class PROJECTBW_API ARangedEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:

	ARangedEnemy();

protected:

	virtual void BeginPlay() override;
};
