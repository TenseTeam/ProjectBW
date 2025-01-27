// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/ResourceAttributeSystem/Components/ResourceAttributeManager.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
#include "IA/EnemyBase/EnemyBase.h"
#include "IA/Weapons/Ranged/RangedWeapon.h"
#include "RangedEnemy.generated.h"

UCLASS()
class PROJECTBW_API ARangedEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:

	ARangedEnemy();
	
	virtual void OnEnemyDead() override;
	
protected:

	virtual void BeginPlay() override;
	
	virtual void PostInitProperties() override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite ,Category = "AI|Ranged")
	bool bIsAttacking = false;
	
	UPROPERTY(EditDefaultsOnly)
	UChildActorComponent* ChildActor;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Ranged")
	ARangedWeapon* RangedWeapon;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "AI|Ranged")
	FName SwordSocketInHand;
};
