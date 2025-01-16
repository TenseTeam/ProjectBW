// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/ResourceAttributeSystem/Components/ResourceAttributeManager.h"

#include "Features/Gameplay/ResourceAttributeSystem/Base/ResourceAttributeBase.h"


UResourceAttributeManager::UResourceAttributeManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UResourceAttributeManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeAttributes();
}

bool UResourceAttributeManager::TryGetAttribute(const FName AttributeName, UResourceAttributeBase*& OutAttribute) const
{
	if (AttributeMap.Contains(AttributeName))
	{
		OutAttribute = AttributeMap[AttributeName];
		return true;
	}
	OutAttribute = nullptr;
	return false;
}

void UResourceAttributeManager::InitializeAttributes()
{
	for (const auto& Elem : AttributeMap)
	{
		Elem.Value->Initialize(GetOwner(), Elem.Key);
	}
}

