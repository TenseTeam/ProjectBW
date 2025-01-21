// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IA/EnemyBase/EnemyBase.h"
#include "IA/Weapons/Melee/MeleeWeapon.h"
#include "MeleeEnemy.generated.h"

UCLASS()
class PROJECTBW_API AMeleeEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	
	AMeleeEnemy();

	UFUNCTION()
	bool GetWieldSword() const {return bWieldSword; }
	
	UFUNCTION()
	void SetWieldSword(bool bHasSword) { bWieldSword = bHasSword; }
	
	UFUNCTION(BlueprintCallable)
	void PlayMontageWithNotify(UAnimMontage* MontageToPlay);
	UFUNCTION(BlueprintCallable)
	void EquipSword(FName SocketName);
protected:
	
	virtual void BeginPlay() override;

	virtual void PostInitProperties() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Melee")
	bool bWieldSword;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Melee")
	FName SwordSocketInHand;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Melee")
	FName SwordSocketOnBack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Melee")
	UAnimMontage* EquipSwordMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Melee")
	UAnimMontage* UnequipSwordMontage;
	
	UPROPERTY(EditDefaultsOnly)
	UChildActorComponent* ChildActor;

	UPROPERTY()
	AMeleeWeapon* MeleeWeapon;


private:
	
	void OnMontageNotifyReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	
};
