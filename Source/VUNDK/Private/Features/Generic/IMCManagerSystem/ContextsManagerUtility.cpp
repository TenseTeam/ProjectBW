// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Generic/IMCManagerSystem/ContextsManagerUtility.h"

UContextsManager* UContextsManagerUtility::ContextsManager = nullptr;

void UContextsManagerUtility::Init(UContextsManager* InContextsManager)
{
	ContextsManager = InContextsManager;
}

UContextsManager* UContextsManagerUtility::GetContextsManager()
{
	return ContextsManager;
}

void UContextsManagerUtility::SetGameplayMappingContext()
{
	if (!Check())
		return;
	
	ContextsManager->SetGameplayMappingContext();
}

void UContextsManagerUtility::SetUIMappingContext()
{
	if (!Check())
		return;
	
	ContextsManager->SetUIMappingContext();
}

bool UContextsManagerUtility::Check()
{
	return IsValid(ContextsManager);
}
