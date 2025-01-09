// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Generic/IMCManagerSystem/ContextsManager.h"

#include "Features/Generic/IMCManagerSystem/ContextsManagerUtility.h"
#include "Kismet/GameplayStatics.h"

UContextsManager::UContextsManager(): GameplayInputsContext(nullptr),
                                      UIInputsContext(nullptr),
                                      PlayerController(nullptr),
                                      EnhancedInputSubsystem(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UContextsManager::SetGameplayMappingContext() const
{
	if (!Check())
		return;
	
	EnhancedInputSubsystem->AddMappingContext(GameplayInputsContext, 0);
	EnhancedInputSubsystem->RemoveMappingContext(UIInputsContext);
	OnGameplayMappingContextSet.Broadcast();
}

void UContextsManager::SetUIMappingContext() const
{
	if (!Check())
		return;
	
	EnhancedInputSubsystem->AddMappingContext(UIInputsContext, 0);
	EnhancedInputSubsystem->RemoveMappingContext(GameplayInputsContext);
	OnUIMappingContextSet.Broadcast();
}

void UContextsManager::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	UContextsManagerUtility::Init(this);
}

bool UContextsManager::Check() const
{
	return IsValid(PlayerController) && IsValid(EnhancedInputSubsystem);
}
