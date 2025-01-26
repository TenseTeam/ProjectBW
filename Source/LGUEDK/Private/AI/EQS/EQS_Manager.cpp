// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQS_Manager.h"

#include "NavigationSystem.h"
#include "VectorTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEQS_Manager::AEQS_Manager()
{
	PrimaryActorTick.bCanEverTick = true;
	CanSearchPoint = false;
	Initi();
}

// Called when the game starts or when spawned
void AEQS_Manager::BeginPlay()
{
	Super::BeginPlay();
	CurrentTime = 0;
	AttackTarget = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	
}

void AEQS_Manager::UpdateStrafePoints()
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return ;
	
	Points.Empty();
	FVector TargetLocation = AttackTarget->GetActorLocation();
	
	float MinRadius = MinAttackRadiusMelee;
	float MaxRadius = MaxAttackRadiusMelee;

	TArray<FVector> MeleePoints;
	FNavLocation NavLocation;
	float GridRadius = MaxRadius;
	float StepSize = 300;
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
	// 	DrawDebugSphere(GetWorld(), Point, 25.0f, 12, FColor::Red, false, UpdateInterval);
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
			2.0f               // Durata della sfera
		);
	}
	
	
	MinRadius = MinAttackRadiusRanged;
	MaxRadius = MaxAttackRadiusRanged;

	TArray<FVector> RangedPoints;
	
	GridRadius = MaxRadius;
	StepSize = 600;
	GridSize = FMath::CeilToInt(GridRadius / StepSize); 
    
	for (int32 X = -GridSize; X <= GridSize; ++X)
	{
		for (int32 Y = -GridSize; Y <= GridSize; ++Y)
		{
			// Punto base della griglia
			FVector GridPoint = TargetLocation + FVector(X * StepSize, Y * StepSize, 0.0f);

			// Aggiunta di randomizzazione attorno al punto base
			float Angle = FMath::DegreesToRadians(FMath::RandRange(0.0f, 100.f));
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
	        2.0f               // Durata della sfera
	    );
	}
	
	
	Points.Add(EEnemyType::Melee, MeleePoints);
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

bool AEQS_Manager::IsInRange(FVector TargetPosition,FVector CurrentPosition, const float MinDistance, const float MaxDistance)
{
	float Distance = FVector::Distance(TargetPosition,CurrentPosition);
	return Distance > MinDistance && Distance < MaxDistance;
}


void AEQS_Manager::Tick(float DeltaTime)
{
	if (CanSearchPoint)
	{
		if (CurrentTime <= 0.0f)
		{
			UpdateStrafePoints();
			CurrentTime = UpdateInterval;
			return;
		}
		CurrentTime -= DeltaTime;
	}
}

FVector AEQS_Manager::GetPoint(EEnemyType EnemyType)
{
	if (Points[EEnemyType::Melee].IsEmpty() || Points[EEnemyType::Ranged].IsEmpty())
	{
		UpdateStrafePoints();
	}
	
	FVector SelectedPoint = FVector::ZeroVector;
	
	TArray<FVector> PossiblePoints;

	if (!Points.Contains(EnemyType))return SelectedPoint;
	
	if (EnemyType == EEnemyType::Melee)
	{
		if (Points[EEnemyType::Melee].IsEmpty())return SelectedPoint;
		PossiblePoints = Points[EEnemyType::Melee];
	}
	else if (EnemyType == EEnemyType::Ranged)
	{
		if (Points[EEnemyType::Ranged].IsEmpty())return SelectedPoint;
		PossiblePoints = Points[EEnemyType::Ranged];
	}

	if (PossiblePoints.Num() > 0)
	{
		int index = FMath::RandRange (0,PossiblePoints.Num() - 1);
		
		SelectedPoint = PossiblePoints[index];
		Points[EnemyType].RemoveAt(index);
	}

	// Debug: verifica se un punto è stato trovato
	if (SelectedPoint == FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid points found for enemy type %d"), (int32)EnemyType);
	}

	return SelectedPoint;
	
}

