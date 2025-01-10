// Copyright VUNDK, Inc. All Rights Reserved.

#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/Utility/StatsUtility.h"
#include "Kismet/GameplayStatics.h"

UStatsBridgeBase* UStatsUtility::GetPlayerPawnStats(int32 PlayerIndex)
{
	const UWorld* World = GEngine->GetWorld();
	
	if (!World)
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("GetPlayerStats(), World is nullptr"));
		return nullptr;
	}

	const APawn* Pawn = UGameplayStatics::GetPlayerPawn(World, 0);

	if (!Pawn)
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("GetPlayerStats(), Character is nullptr"));
		return nullptr;
	}

	UStatsBridgeBase* CharacterStats = Pawn->FindComponentByClass<UStatsBridgeBase>();
	return CharacterStats;
}
