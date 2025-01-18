// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/ResourceAttributeSystem/Components/ResourceAttributeManager.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
#include "IA/EnemyBase/EnemyBase.h"
#include "RangedEnemy.generated.h"

UCLASS()
class PROJECTBW_API ARangedEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:

	ARangedEnemy();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "AI|Shooter")
	void BaseAttack();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Shooter")
	UShooter* Shooter;


	

};
