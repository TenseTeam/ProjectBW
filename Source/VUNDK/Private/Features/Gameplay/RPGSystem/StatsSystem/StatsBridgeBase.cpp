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

void UStatsBridgeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SpecialStatsContainer->OnStatsValuesChanged.RemoveDynamic(this, &UStatsBridgeBase::CalculateAllStatsValues);
	CoreStatsContainer->OnStatsValuesChanged.RemoveDynamic(this, &UStatsBridgeBase::CalculateFullStatsValues);
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
	
	if (!BridgeSaveData)
		return false;

	for (auto& SpecialStatsMap = BridgeSaveData->SavedSpecialStats; auto& Pair : SpecialStatsMap)
	{
		if (USpecialStatData* SpecialStatData = GetSpecialStatByID(Pair.Key))
			SpecialStatsContainer->AddStat(SpecialStatData, Pair.Value);
	}
	
	for (auto& CoreStatsMap = BridgeSaveData->SavedCoreStats; auto& Pair : CoreStatsMap)
	{
		if (UCoreStatData* CoreStatData = GetCoreStatByID(Pair.Key))
			CoreStatsContainer->AddStat(CoreStatData, Pair.Value);
	}
	
	return true;
}

USpecialStatData* UStatsBridgeBase::GetSpecialStatByID(const FGuid SpecialStatID) const
{
	for (USpecialStatData* SpecialStat : SpecialStats)
	{
		if (SpecialStat->StatID == SpecialStatID)
			return SpecialStat;
	}

	return nullptr;
}

UCoreStatData* UStatsBridgeBase::GetCoreStatByID(const FGuid CoreStatID) const
{
	for (UCoreStatData* CoreStatData : CoreStats)
	{
		if (CoreStatData->StatID == CoreStatID)
			return CoreStatData;
	}

	return nullptr;
}

TMap<UCoreStatData*, float> UStatsBridgeBase::GetFullStatsValues() const
{
	return FullStatsContainer->GetCoreStatsValues();
}

float UStatsBridgeBase::GetFullStatValueAsFloat(const UStatDataBase* Stat) const
{
	return FullStatsContainer->GetValueAsFloat(Stat);
}

int32 UStatsBridgeBase::GetFullStatValueAsInt(const UStatDataBase* Stat) const
{
	return FullStatsContainer->GetValueAsInt(Stat);
}

FString UStatsBridgeBase::GetFullStatValueAsString(const UStatDataBase* Stat) const
{
	return FullStatsContainer->GetValueAsString(Stat);
}

int32 UStatsBridgeBase::GetFullStatsLength() const
{
	return FullStatsContainer->GetStatsLength();
}

void UStatsBridgeBase::CalculateCoreStatValueWithSpecialStat(USpecialStatData* SpecialStatData, UCoreStatData* CoreStatData, const TSubclassOf<UStatOperation> OperationClass)
{
	if (!IsValid(SpecialStatData) || !IsValid(CoreStatData))
		return;
	
	const UStatOperation* Operation = URPGFactory::CreateBridgeStatOperation(this, SpecialStatData, OperationClass);
	
	if (!IsValid(Operation))
		return;

	CoreStatsContainer->TryModifyValue(CoreStatData, Operation->GetResultOperation(), false);
}

void UStatsBridgeBase::CalculateAllStatsValues()
{
	CalculateCoreStatsValues();
	CalculateFullStatsValues();
}

void UStatsBridgeBase::BeginPlay()
{
	CreateStatsContainers();
	SpecialStatsContainer->AddSpecialStats(SpecialStats);
	CoreStatsContainer->AddCoreStats(CoreStats);
	Super::BeginPlay();
	SpecialStatsContainer->OnStatsValuesChanged.AddDynamic(this, &UStatsBridgeBase::CalculateAllStatsValues);
	CoreStatsContainer->OnStatsValuesChanged.AddDynamic(this, &UStatsBridgeBase::CalculateFullStatsValues);
}

void UStatsBridgeBase::SetFullStatValue(UStatDataBase* Stat, const float Value) const
{
	FullStatsContainer->TrySetValue(Stat, Value);
}

void UStatsBridgeBase::ModifyFullStatValue(UStatDataBase* Stat, const float Value) const
{
	FullStatsContainer->TryModifyValue(Stat, Value);
}

void UStatsBridgeBase::CalculateCoreStatsValues()
{
	OnCalculateCoreStatsValues();
	OnCalculatedCoreStatsValues.Broadcast(this);
}

void UStatsBridgeBase::CalculateFullStatsValues()
{
	FullStatsContainer->CopyStats(CoreStatsContainer); // By default, full values are equal to core values
	OnCalculateFullStatsValues();
	OnCalculatedFullStatsValues.Broadcast(this);
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
