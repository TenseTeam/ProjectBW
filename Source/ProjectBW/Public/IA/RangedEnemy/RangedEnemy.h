// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Shooter")
	UShooter* Shooter;
};
