// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Interfaces/AITargetInterface.h"
#include "Components/ActorComponent.h"
#include "EnemyAttackRequestManager.generated.h"


UCLASS(Blueprintable,BlueprintType,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LGUEDK_API UEnemyAttackRequestManager : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UEnemyAttackRequestManager();

	UFUNCTION(BlueprintPure,BlueprintCallable)
	int GetTeamIndex() const {return TeamIndex;}

	UFUNCTION(BlueprintCallable)
	bool TryGetToken(int TokenCost);
	
	UFUNCTION(BlueprintCallable)
	void ReleaseToken(int ReturnToken);

	UFUNCTION()
	void SetTeamIndex(int NewTeamIndex) {TeamIndex = NewTeamIndex;}
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int TeamIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int MaxTokens;
	
private:
	
	UPROPERTY()
	int CurrentTokens;

};
