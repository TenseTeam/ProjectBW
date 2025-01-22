// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataProviders/AIDataProvider.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "EQS_Strafing.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class LGUEDK_API UEQS_Strafing : public UEnvQueryGenerator
{
	GENERATED_BODY()

public:
	
	UEQS_Strafing();

protected:
	
	// Genera i punti attorno al target
	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	// Contesto per il target (es. il player o l'attore da attaccare)
	UPROPERTY(EditAnywhere, Category = "Generator Settings")
	TSubclassOf<UEnvQueryContext> CenterContext;

	// Numero di punti da generare attorno al target
	UPROPERTY(EditDefaultsOnly, Category = "Generator Settings")
	FAIDataProviderIntValue NumberOfPoints;

	// Raggio minimo dal target per i punti generati
	UPROPERTY(EditDefaultsOnly, Category = "Generator Settings")
	FAIDataProviderFloatValue MinRadius;

	// Raggio massimo dal target per i punti generati
	UPROPERTY(EditDefaultsOnly, Category = "Generator Settings")
	FAIDataProviderFloatValue MaxRadius;
};
