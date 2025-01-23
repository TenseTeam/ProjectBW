// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQS_Manager.h"

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
	
}

void AEQS_Manager::UpdateStrafePoints()
{
	Points.Empty();
	ACharacter* AttackTarget = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	FVector TargetLocation = AttackTarget->GetActorLocation();
	
	float MinRadius = MinAttackRadiusMelee;
	float MaxRadius = MaxAttackRadiusMelee;

	TArray<FVector> MeleePoints;
	
	for (int32 i = 0; i < NumberOfPoints; i++)
	{
		float Angle = FMath::DegreesToRadians(360.0f / NumberOfPoints * i);
		float Radius = FMath::RandRange(MinRadius, MaxRadius);

		FVector Point = TargetLocation + FVector(
			FMath::Cos(Angle) * Radius,
			FMath::Sin(Angle) * Radius,
			0.0f
		);

		MeleePoints.Add(Point);
		
		DrawDebugSphere(GetWorld(), Point, 25.0f, 12, FColor::Red, false, UpdateInterval);
	}
	
	MinRadius = MinAttackRadiusRanged;
	MaxRadius = MaxAttackRadiusRanged;

	TArray<FVector> RangedPoints;
	
	for (int32 i = 0; i < NumberOfPoints; i++)
	{
		float Angle = FMath::DegreesToRadians(360.0f / NumberOfPoints * i);
		float Radius = FMath::RandRange(MinRadius, MaxRadius);

		FVector Point = TargetLocation + FVector(
			FMath::Cos(Angle) * Radius,
			FMath::Sin(Angle) * Radius,
			0.0f
		);

		RangedPoints.Add(Point);
		
		DrawDebugSphere(GetWorld(), Point, 25.0f, 12, FColor::Blue, false, UpdateInterval);
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

// Called every frame
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
		int index = FMath::RandRange (0,PossiblePoints.Num());
		
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

