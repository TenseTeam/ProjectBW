// Copyright Epic Games, Inc. All Rights Reserved.

#include "GVUEDK.h"

#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FGVUEDKModule"

void FGVUEDKModule::StartupModule()
{
	const FName TagName = FName("PlayerAction");
	if (!UGameplayTagsManager::Get().RequestGameplayTag(TagName, false).IsValid())
	{
		UGameplayTagsManager::Get().AddNativeGameplayTag(TagName, TEXT("Player Action Tags"));
	}
}

void FGVUEDKModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGVUEDKModule, GVUEDK)
