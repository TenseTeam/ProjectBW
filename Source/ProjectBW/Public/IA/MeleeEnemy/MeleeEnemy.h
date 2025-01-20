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

	UFUNCTION()
	bool GetWieldSword() const {return bWieldSword; }
	UFUNCTION()
	void SetWieldSword(bool bHasSword) { bWieldSword = bHasSword; }
	UFUNCTION()
	void EquipSword();
	UFUNCTION()
	void UnequipSword();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Melee")
	bool bWieldSword;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Melee")
	FName SwordSocketInHand;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Melee")
	FName SwordSocketOnBack;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI|Melee")
	UAnimMontage* EquipSwordMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Melee")
	UAnimMontage* UnequipSwordMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI|Melee")
	UStaticMesh* SwordMesh;

private:
	
	void PlayMontageWithNotify(UAnimMontage* MontageToPlay, const FName& NotifyName, const FName& TargetSocket);
	void OnMontageNotifyReceived(const FName& NotifyName, const FName& TargetSocket);
	
};
