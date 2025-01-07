// Copyright VUNDK, Inc. All Rights Reserved.

#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/StatsBridgeBase.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/SaveData/StatsBridgeSaveData.h"
#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/Data/SaveData/StatsSaveData.h"

UStatsBridgeBase::UStatsBridgeBase()
{
	PrimaryComponentTick.bCanEverTick = false;
	SpecialStatsContainer = CreateDefaultSubobject<USpecialStatsContainer>(TEXT("SpecialStats"));
	BaseStatsContainer = CreateDefaultSubobject<UBaseStatsContainer>(TEXT("BaseStats"));
}

void UStatsBridgeBase::BeginPlay()
{
	SpecialStatsContainer->Init();
	BaseStatsContainer->Init();
	CalculateBaseStatsValues();
	Super::BeginPlay();
	SpecialStatsContainer->OnSpecialStatDefaultValueChanged.AddDynamic(this, &UStatsBridgeBase::CalculateAllStatsValues);
}

void UStatsBridgeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SpecialStatsContainer->OnSpecialStatDefaultValueChanged.RemoveDynamic(this, &UStatsBridgeBase::CalculateAllStatsValues);
}

USaveData* UStatsBridgeBase::CreateSaveData()
{
	UStatsBridgeSaveData* BridgeSaveData = NewObject<UStatsBridgeSaveData>();
	
	BridgeSaveData->BaseStatsSaveData = Cast<UStatsSaveData>(BaseStatsContainer->CreateSaveData());
	BridgeSaveData->SpecialStatsSaveData = Cast<UStatsSaveData>(SpecialStatsContainer->CreateSaveData());
	return BridgeSaveData;
}

bool UStatsBridgeBase::LoadSaveData(USaveData* SavedData)
{
	if (!SavedData)
		return false;

	const UStatsBridgeSaveData* BridgeSaveData = Cast<UStatsBridgeSaveData>(SavedData);

	BaseStatsContainer->LoadSaveData(BridgeSaveData->BaseStatsSaveData);
	SpecialStatsContainer->LoadSaveData(BridgeSaveData->SpecialStatsSaveData);
	return true;
}

TMap<UBaseStatData*, int32> const& UStatsBridgeBase::GetFullStatsValues() const
{
	return FullValues;
}

void UStatsBridgeBase::SetStatBaseValueBySpecialStat(USpecialStatData* SpecialStatData, UBaseStatData* BaseStatData)
{
	if (!IsValid(SpecialStatData) || !IsValid(BaseStatData))
		return;

	if (!SpecialStatsContainer->SpecialStats.Contains(SpecialStatData))
		return;
	
	const UStatOperation* Operation = URPGFactory::CreateBridgeStatOperation(this, SpecialStatData);
	
	if (!IsValid(Operation))
		return;

	BaseStatsContainer->SetStatBaseValue(BaseStatData, Operation->GetResultOperation());
}

void UStatsBridgeBase::CalculateAllStatsValues()
{
	OnCalculateBaseStatsValues();
	OnCalculateFullStatsValues();
}

void UStatsBridgeBase::CalculateBaseStatsValues()
{
	OnCalculateBaseStatsValues();
	OnBaseStatsValuesChanged.Broadcast();
}

void UStatsBridgeBase::CalculateFullStatsValues()
{
	FullValues = BaseStatsContainer->GetBaseValues(); // By default, full values are equal to base values
	OnCalculateFullStatsValues();
	OnFullStatsValuesChanged.Broadcast();
}

void UStatsBridgeBase::OnCalculateBaseStatsValues_Implementation()
{
}

void UStatsBridgeBase::OnCalculateFullStatsValues_Implementation()
{
}
