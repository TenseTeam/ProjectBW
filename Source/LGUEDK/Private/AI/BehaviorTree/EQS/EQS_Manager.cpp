// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQS_Manager.h"

#include "NavigationSystem.h"
#include "VectorTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/LGDebug.h"


// Sets default values
AEQS_Manager::AEQS_Manager()
{
	PrimaryActorTick.bCanEverTick = true;
	CanSearchMeleePoint = false;
	Initi();
}

// Called when the game starts or when spawned
void AEQS_Manager::BeginPlay()
{
	Super::BeginPlay();
	CurrentMeleeTime = 0;
	CurrentRangedTime = 0;
	AttackTarget = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	
}

void AEQS_Manager::UpdateStrafeMeleePoints()
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return ;
	
	FVector TargetLocation = AttackTarget->GetActorLocation();
	
	float MinRadius = MinAttackRadiusMelee;
	float MaxRadius = MaxAttackRadiusMelee;

	TArray<FVector> MeleePoints;
	FNavLocation NavLocation;
	float GridRadius = MaxRadius;
	float StepSize = 150;
	int32 GridSize = FMath::CeilToInt(GridRadius / StepSize);

	// for (int32 i = 0; i < NumberOfPoints; i++)
	// {
	// 	float Angle = FMath::DegreesToRadians(360.0f / NumberOfPoints * i);
	// 	float Radius = FMath::RandRange(MinRadius, MaxRadius);
	//
	// 	FVector Point = TargetLocation + FVector(
	// 		FMath::Cos(Angle) * Radius,
	// 		FMath::Sin(Angle) * Radius,
	// 		0.0f
	// 	);
	// 	
	// 	MeleePoints.Add(Point);
	// 	DrawDebugSphere(GetWorld(), Point, 25.0f, 12, FColor::Red, false, UpdateMeleeInterval);
	// }
	
	for (int32 X = -GridSize; X <= GridSize; ++X)
	{
		for (int32 Y = -GridSize; Y <= GridSize; ++Y)
		{
			FVector CandidatePoint = TargetLocation + FVector(X * StepSize, Y * StepSize, 0.0f);
			
			if (NavSystem->ProjectPointToNavigation(CandidatePoint, NavLocation, FVector(StepSize, StepSize, 200.0f)))
			{
				
				if (IsInRange(TargetLocation,NavLocation.Location,MinRadius,MaxRadius) && !IsHittingSomething(NavLocation.Location,TargetLocation))
					MeleePoints.Add(NavLocation.Location);
				
				//LGDebug::Log("Found Reachable Point: " + NavLocation.Location.ToString(), true);
			}
		}
	}
	
	for (const FVector& Point : MeleePoints)
	{
		DrawDebugSphere(
			GetWorld(), 
			Point,				// Posizione della sfera
			50.0f,             // Raggio della sfera
			12,                // Numero di segmenti
			FColor::Red,     // Colore della sfera
			false,             // Persistente nel tempo
			UpdateMeleeInterval            // Durata della sfera
		);
	}
	
	Points.Add(EEnemyType::Melee, MeleePoints);
}

void AEQS_Manager::UpdateStrafeRangedPoints()
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return ;
	
	FVector TargetLocation = AttackTarget->GetActorLocation();
	
	float MinRadius = MinAttackRadiusRanged;
	float MaxRadius = MaxAttackRadiusRanged;

	TArray<FVector> RangedPoints;
	FNavLocation NavLocation;
	float GridRadius = MaxRadius;
	float StepSize = 600;
	int32 GridSize = FMath::CeilToInt(GridRadius / StepSize); 
    
	for (int32 X = -GridSize; X <= GridSize; ++X)
	{
		for (int32 Y = -GridSize; Y <= GridSize; ++Y)
		{
			// Punto base della griglia
			FVector GridPoint = TargetLocation + FVector(X * StepSize, Y * StepSize, 0.0f);

			// Aggiunta di randomizzazione attorno al punto base
			float Angle = FMath::DegreesToRadians(FMath::RandRange(0.0f, 360.f));
			float Radius = FMath::RandRange(0.0f, StepSize); // Randomizzazione entro la dimensione della griglia
			FVector RandomOffset = FVector(
				FMath::Cos(Angle) * Radius,
				FMath::Sin(Angle) * Radius,
				0.0f
			);

			// Somma il punto randomizzato al punto della griglia
			FVector CandidatePoint = GridPoint + RandomOffset;
			
			// FNavLocation NavLocation;
			if (NavSystem->ProjectPointToNavigation(CandidatePoint, NavLocation, FVector(StepSize, StepSize, 200.0f)))
			{
				if (IsInRange(TargetLocation,NavLocation.Location,MinRadius,MaxRadius) && !IsHittingSomething(NavLocation.Location,TargetLocation))
					RangedPoints.Add(NavLocation.Location);
				
				//LGDebug::Log("Found Reachable Point: " + NavLocation.Location.ToString(), true);
			}
		}
	}
	
	for (const FVector& Point : RangedPoints)
	{
		DrawDebugSphere(
			GetWorld(), 
			Point,				// Posizione della sfera
			50.0f,             // Raggio della sfera
			12,                // Numero di segmenti
			FColor::Green,     // Colore della sfera
			false,             // Persistente nel tempo
			UpdateRangedInterval               // Durata della sfera
		);
	}
	
	Points.Add(EEnemyType::Ranged, RangedPoints);
}

void AEQS_Manager::Initi()
{
	Points = TMap<EEnemyType,TArray<FVector>>();
	Points.Add(EEnemyType::Melee);
	Points.Add(EEnemyType::Ranged);
}

bool AEQS_Manager::IsHittingSomething(const FVector& Start, const FVector& End)
{
	FHitResult HitResult;

	FVector StartLocation = Start ;
	FVector EndLocation = End ;
    
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(AttackTarget);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, 
		StartLocation,
		EndLocation,
		ECC_Visibility,
		TraceParams
	);
	
	return bHit;
}

bool AEQS_Manager::IsInRange(FVector& TargetPosition,FVector& CurrentPosition,const float& MinDistance, const float& MaxDistance)
{
	float Distance = FVector::Distance(TargetPosition,CurrentPosition);
	return Distance > MinDistance && Distance < MaxDistance;
}


void AEQS_Manager::Tick(float DeltaTime)
{
	if (CanSearchMeleePoint)
	{
		if (CurrentMeleeTime <= 0.0f)
		{
			Points[EEnemyType::Melee].Empty();
			UpdateStrafeMeleePoints();
			CurrentMeleeTime = UpdateMeleeInterval;
			return;
		}
		CurrentMeleeTime -= DeltaTime;
	}

	if (CanSearchRengedPoint)
	{
		if (CurrentRangedTime <= 0.0f)
		{
			Points[EEnemyType::Ranged].Empty();
			UpdateStrafeRangedPoints();
			CurrentRangedTime = UpdateMeleeInterval;
		}
		CurrentRangedTime -= DeltaTime;
	}
	
}

FVector AEQS_Manager::GetPoint(EEnemyType& EnemyType)
{
	FVector SelectedPoint = FVector::ZeroVector;
	
	TArray<FVector> PossiblePoints;
	
	switch (EnemyType) {
	case EEnemyType::Melee:
		
		if (Points[EEnemyType::Melee].Num() <= 0)
			UpdateStrafeMeleePoints();
		
		PossiblePoints = Points[EEnemyType::Melee];
		CanSearchMeleePoint = true;
		
		break;
	case EEnemyType::Ranged:
		
		if(Points[EEnemyType::Ranged].Num() <= 0)
			UpdateStrafeRangedPoints();
		
		PossiblePoints = Points[EEnemyType::Ranged];
		CanSearchRengedPoint = true;
		break;
	}

	if (PossiblePoints.Num() > 0)
	{
		int index = FMath::RandRange (0,PossiblePoints.Num() - 1);
		
		SelectedPoint = PossiblePoints[index];
		Points[EnemyType].RemoveAt(index);
	}

	
	if (SelectedPoint == FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid points found for enemy type %d"), (int32)EnemyType);
	}

	return SelectedPoint;
	
}

