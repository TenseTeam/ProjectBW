// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IA/EnemyBase/EnemyBase.h"
#include "IA/Weapons/Melee/MeleeWeapon.h"
#include "MeleeEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FFinishedAnimation);


UCLASS()
class PROJECTBW_API AMeleeEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FFinishedAnimation OnFinishedAnimation;
	
	AMeleeEnemy();

	UFUNCTION(Blueprintable, BlueprintCallable)
	bool GetWieldSword() const {return bWieldSword; }
	
	UFUNCTION(Blueprintable, BlueprintCallable)
	void SetWieldSword(bool bHasSword) { bWieldSword = bHasSword; }
	
	UFUNCTION(BlueprintCallable)
	void EquipSword(FName SocketName);

	UFUNCTION(BlueprintCallable)
	void FinishedAnimation();

	virtual void OnEnemyDead_Implementation() override;
	
protected:
	
	virtual void BeginPlay() override;

	virtual void PostInitProperties() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "AI|Melee")
	FName SwordSocketInHand;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "AI|Melee")
	FName SwordSocketOnBack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Melee")
	UAnimMontage* EquipSwordMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Melee")
	UAnimMontage* UnequipSwordMontage;
	
	UPROPERTY(EditDefaultsOnly)
	UChildActorComponent* ChildActor;
	
	UPROPERTY(BlueprintReadOnly, Category = "AI|Melee")
	AMeleeWeapon* MeleeWeapon;

private:
	
	UPROPERTY(EditAnywhere, Category = "AI|Melee")
	bool bWieldSword;
	
	
};
