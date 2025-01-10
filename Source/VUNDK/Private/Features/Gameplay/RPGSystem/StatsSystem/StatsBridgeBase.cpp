// Copyright VUNDK, Inc. All Rights Reserved.

#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/StatsBridgeBase.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/SaveData/StatsBridgeSaveData.h"

UStatsBridgeBase::UStatsBridgeBase(): SpecialStatsContainer(nullptr),
                                      CoreStatsContainer(nullptr),
                                      FullStatsContainer(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatsBridgeBase::BeginPlay()
{
	CreateStatsContainers();
	SpecialStatsContainer->AddSpecialStats(SpecialStats);
	CoreStatsContainer->AddCoreStats(CoreStats);
	CalculateCoreStatsValues();
	Super::BeginPlay();
	SpecialStatsContainer->OnStatsValuesChanged.AddDynamic(this, &UStatsBridgeBase::CalculateAllStatsValues);
}

void UStatsBridgeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SpecialStatsContainer->OnStatsValuesChanged.RemoveDynamic(this, &UStatsBridgeBase::CalculateAllStatsValues);
}

USaveData* UStatsBridgeBase::CreateSaveData()
{
	UStatsBridgeSaveData* BridgeSaveData = NewObject<UStatsBridgeSaveData>();

	for (auto& Pair : SpecialStatsContainer->GetValues())
		BridgeSaveData->SavedSpecialStats.Add(Pair.Key->StatID, Pair.Value);
	
	for (auto& Pair : CoreStatsContainer->GetValues())
		BridgeSaveData->SavedCoreStats.Add(Pair.Key->StatID, Pair.Value);
	
	return BridgeSaveData;
}

bool UStatsBridgeBase::LoadSaveData(USaveData* SavedData)
{
	if (!SavedData)
		return false;

	const UStatsBridgeSaveData* BridgeSaveData = Cast<UStatsBridgeSaveData>(SavedData);

	for (TMap<FName, int32> SpecialStatsMap = BridgeSaveData->SavedSpecialStats; auto& Pair : SpecialStatsMap)
	{
		if (USpecialStatData* SpecialStatData = GetSpecialStatByID(Pair.Key))
			SpecialStatsContainer->TrySetValue(SpecialStatData, Pair.Value);
	}
	
	for (TMap<FName, int32> CoreStatsMap = BridgeSaveData->SavedCoreStats; auto& Pair : CoreStatsMap)
	{
		if (UCoreStatData* CoreStatData = GetCoreStatByID(Pair.Key))
			CoreStatsContainer->TrySetValue(CoreStatData, Pair.Value);
	}
	
	return true;
}

UCoreStatData* UStatsBridgeBase::GetCoreStatByID(const FName& BaseStatID) const
{
	for (UCoreStatData* CoreStatData : CoreStats)
	{
		if (CoreStatData->StatID.IsEqual(BaseStatID))
			return CoreStatData;
	}

	return nullptr;
}

USpecialStatData* UStatsBridgeBase::GetSpecialStatByID(const FName& SpecialStatID) const
{
	for (USpecialStatData* SpecialStat : SpecialStats)
	{
		if (SpecialStat->StatID.IsEqual(SpecialStatID))
			return SpecialStat;
	}

	return nullptr;
}

void UStatsBridgeBase::CalculateCoreStatValueWithSpecialStat(USpecialStatData* SpecialStatData, UCoreStatData* CoreStatData, const TSubclassOf<UStatOperation> OperationClass)
{
	if (!IsValid(SpecialStatData) || !IsValid(CoreStatData))
		return;
	
	const UStatOperation* Operation = URPGFactory::CreateBridgeStatOperation(this, SpecialStatData, OperationClass);
	
	if (!IsValid(Operation))
		return;

	CoreStatsContainer->TrySetValue(CoreStatData, Operation->GetResultOperation());
}

void UStatsBridgeBase::CalculateAllStatsValues()
{
	CalculateCoreStatsValues();
	CalculateFullStatsValues();
}

void UStatsBridgeBase::CalculateCoreStatsValues()
{
	OnCalculateCoreStatsValues();
	OnCalculatedCoreStatsValues.Broadcast();
}

void UStatsBridgeBase::CalculateFullStatsValues()
{
	FullStatsContainer->CopyStats(CoreStatsContainer); // By default, full values are equal to core values
	OnCalculateFullStatsValues();
	OnCalculatedFullStatsValues.Broadcast(FullStatsContainer);
}

void UStatsBridgeBase::OnCalculateCoreStatsValues_Implementation()
{
}

void UStatsBridgeBase::OnCalculateFullStatsValues_Implementation()
{
}

void UStatsBridgeBase::CreateStatsContainers()
{
	SpecialStatsContainer = NewObject<USpecialStatsContainer>(this, TEXT("SpecialStatsContainer"));
	CoreStatsContainer = NewObject<UCoreStatsContainer>(this, TEXT("CoreStatsContainer"));
	FullStatsContainer = NewObject<UCoreStatsContainer>(this, TEXT("FullStatsContainer"));
}
