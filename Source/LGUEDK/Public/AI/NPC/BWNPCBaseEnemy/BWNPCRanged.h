// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BWNPCBaseEnemy.h"
#include "BWNPCRanged.generated.h"

UCLASS()
class LGUEDK_API ABWNPCRanged : public ABWNPCBaseEnemy
{
	GENERATED_BODY()

public:
	ABWNPCRanged();
	float GetRandomRadius() override { return FMath::RandRange(MinAttackRadius, MaxAttackRadius); }
	float GetRandomStrafeRadius() override { return FMath::RandRange(MinStrafeRadius, MaxStrafeRadius); }
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Ranges")
	float MinAttackRadius = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Ranges")
	float MaxAttackRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Ranges")
	float MinStrafeRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Ranges")
	float MaxStrafeRadius = 1000.f;
	
};
