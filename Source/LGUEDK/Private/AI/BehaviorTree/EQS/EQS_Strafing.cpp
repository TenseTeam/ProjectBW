// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQS_Strafing.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

UEQS_Strafing::UEQS_Strafing()
{
	// Specifica il tipo di oggetto generato: punti nello spazio
	ItemType = UEnvQueryItemType_Point::StaticClass();

	// Valori di default per i parametri
	NumberOfPoints.DefaultValue = 8; // Numero di punti predefinito
	MinRadius.DefaultValue = 300.0f; // Raggio minimo predefinito
	MaxRadius.DefaultValue = 600.0f; // Raggio massimo predefinito

	// Context di default: il giocatore che l'IA sta attaccando
	CenterContext = UEnvQueryContext_Querier::StaticClass();
}

// Metodo principale per generare i punti
void UEQS_Strafing::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
		// Variabili per memorizzare i valori dei parametri
	float MinRadiusValue = MinRadius.DefaultValue;
	float MaxRadiusValue = MaxRadius.DefaultValue;
	int32 NumPointsValue = NumberOfPoints.DefaultValue;

	// Ottieni il contesto (attori di riferimento) dal QueryInstance
	TArray<AActor*> ContextActors;
	QueryInstance.PrepareContext(CenterContext, ContextActors);

	// Verifica che ci sia almeno un attore valido nel contesto
	if (ContextActors.Num() == 0 || !ContextActors[0])
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS_StrafeAroundTarget: No valid context actors found."));
		return;
	}

	// Ottieni la posizione del primo attore nel contesto
	AActor* CenterActor = ContextActors[0];
	FVector CenterLocation = CenterActor->GetActorLocation();

	// Assicurati che il raggio massimo sia maggiore o uguale al minimo
	MaxRadiusValue = FMath::Max(MaxRadiusValue, MinRadiusValue);

	// Array per memorizzare i punti generati
	TArray<FNavLocation> GeneratedLocations;

	// Calcola l'angolo tra i punti in radianti
	float AngleStep = 2 * PI / NumPointsValue;

	// Aggiungiamo una variabile per il punto scelto (se ce ne sono punti validi)
	FVector SelectedPoint;

	// Genera i punti attorno al target
	for (int32 i = 0; i < NumPointsValue; i++)
	{
		// Calcola l'angolo corrente
		float Angle = i * AngleStep;

		// Genera un raggio casuale tra il minimo e il massimo
		float Radius = FMath::RandRange(MinRadiusValue, MaxRadiusValue);

		// Calcola la posizione del punto nello spazio attorno al target
		FVector PointLocation = CenterLocation + FVector(
			FMath::Cos(Angle) * Radius, // Spostamento sull'asse X
			FMath::Sin(Angle) * Radius, // Spostamento sull'asse Y
			0.0f                        // Mantieni la Z invariata
		);

		// Aggiungi il punto generato alla lista
		GeneratedLocations.Add(FNavLocation(PointLocation));

		// Supponiamo che tu voglia settare il primo punto valido trovato nella blackboard
		if (SelectedPoint.IsZero())  // Se non è stato ancora selezionato un punto valido
		{
			SelectedPoint = PointLocation;
		}
	}

	// Se è stato selezionato un punto valido, impostalo nella Blackboard
	if (!SelectedPoint.IsZero())
	{
		// Assicurati che l'AI abbia una reference alla Blackboard
		UBlackboardComponent* BlackboardComp = CenterActor->FindComponentByClass<UBlackboardComponent>();
		if (BlackboardComp)
		{
			// Imposta il valore di "TargetLocation" nella Blackboard
			BlackboardComp->SetValueAsVector("TargetLocation", SelectedPoint);
		}
	}

	// Restituisci i punti generati all'EQS
	QueryInstance.AddItemData<UEnvQueryItemType_Point>(GeneratedLocations);
}