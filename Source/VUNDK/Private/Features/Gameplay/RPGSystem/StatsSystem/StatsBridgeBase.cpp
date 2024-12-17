// Copyright VUNDK, Inc. All Rights Reserved.

#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/StatsBridgeBase.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/SaveData/StatsBridgeSaveData.h"
#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/Data/SaveData/StatsSaveData.h"

UStatsBridgeBase::UStatsBridgeBase()
{
	PrimaryComponentTick.bCanEverTick = false;
	SpecialStats = CreateDefaultSubobject<USpecialStatsContainer>(TEXT("SpecialStats"));
	BaseStats = CreateDefaultSubobject<UBaseStatsContainer>(TEXT("BaseStats"));
}

void UStatsBridgeBase::BeginPlay()
{
	SpecialStats->Init();
	BaseStats->Init();
	Super::BeginPlay();
}

USaveData* UStatsBridgeBase::CreateSaveData()
{
	UStatsBridgeSaveData* BridgeSaveData = NewObject<UStatsBridgeSaveData>();
	
	BridgeSaveData->BaseStatsSaveData = Cast<UStatsSaveData>(BaseStats->CreateSaveData());
	BridgeSaveData->SpecialStatsSaveData = Cast<UStatsSaveData>(SpecialStats->CreateSaveData());
	return BridgeSaveData;
}

bool UStatsBridgeBase::LoadSaveData(USaveData* SavedData)
{
	if (!SavedData)
		return false;

	const UStatsBridgeSaveData* BridgeSaveData = Cast<UStatsBridgeSaveData>(SavedData);

	BaseStats->LoadSaveData(BridgeSaveData->BaseStatsSaveData);
	SpecialStats->LoadSaveData(BridgeSaveData->SpecialStatsSaveData);
	return true;
}

void UStatsBridgeBase::CalculateBaseStatAffectedBySpecialStat(const TSubclassOf<UStatOperation> OperationClass, USpecialStatData* SpecialStatData, UBaseStatData* BaseStatData)
{
	const UStatOperation* Operation = URPGFactory::CreateStatOperationForBridge(OperationClass, this, SpecialStatData);
	
	if (!IsValid(Operation))
		return;

	BaseStats->SetStatBaseValue(BaseStatData, Operation->GetResultOperation());
}
