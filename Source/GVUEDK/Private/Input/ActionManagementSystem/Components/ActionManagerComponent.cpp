// Copyright Villains, Inc. All Rights Reserved.


#include "Input/ActionManagementSystem/Components/ActionManagerComponent.h"

#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Input/ActionManagementSystem/PlayerAction.h"
#include "Utility/FGvDebug.h"

DEFINE_LOG_CATEGORY(ActionManagerLog);

// Sets default values for this component's properties
UActionManagerComponent::UActionManagerComponent() : PlayerController(nullptr), bInitialized(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionManagerComponent::BeginPlay()
{
	Initialize();
	Super::BeginPlay();
}

bool UActionManagerComponent::TryGetActioin(const FGameplayTag ActionTag, UPlayerAction*& Action) const
{
	if (!bInitialized) return false;

	if (!PlayerActionMap.Contains(ActionTag))
	{
		FGvDebug::Warning(ActionTag.ToString() + " Action not found in PlayerActionsMap");
		return false;
	}

	Action = PlayerActionMap[ActionTag];
	if (!IsValid(Action))
	{
		FGvDebug::Warning(ActionTag.ToString() + " Action is invalid");
		return false;
	}

	return true;
}

bool UActionManagerComponent::TryBindToAction(const FGameplayTag ActionTag, const FActionTriggered& Callback)
{
	if (!bInitialized)
	{
		FGvDebug::Warning("ActionManagerComponent is not initialized properly");
		return false;
	}

	UPlayerAction* Action;
	if (!TryGetActioin(ActionTag, Action)) return false;

	Action->OnActionTriggered.Add(Callback);
	return true;
}

bool UActionManagerComponent::WantPerform(const FGameplayTag ActionTag) const
{
	if (!bInitialized) return false;

	UPlayerAction* Action;
	if (!TryGetActioin(ActionTag, Action)) return false;

	return Action->WantPerform();
}

bool UActionManagerComponent::IsPerforming(const FGameplayTag ActionTag) const
{
	if (!bInitialized) return false;

	UPlayerAction* Action;
	if (!TryGetActioin(ActionTag, Action)) return false;

	return Action->IsPerforming();
}

bool UActionManagerComponent::CanPerform(const FGameplayTag ActionTag) const
{
	if (!bInitialized) return false;

	UPlayerAction* Action;
	if (!TryGetActioin(ActionTag, Action)) return false;

	return Action->CanPerform();
}

void UActionManagerComponent::SetCanPerform(const FGameplayTag ActionTag, const bool Value)
{
	CHECK_INITIALIZATION

	UPlayerAction* Action;
	if (!TryGetActioin(ActionTag, Action)) return;

	Action->SetCanPerform(Value);
}

void UActionManagerComponent::SetIsPerforming(const FGameplayTag ActionTag, const bool Value)
{
	CHECK_INITIALIZATION

	UPlayerAction* Action;
	if (!TryGetActioin(ActionTag, Action)) return;

	Action->SetIsPerforming(Value);
}

void UActionManagerComponent::Initialize()
{
	PlayerController = Cast<APlayerController>(GetOwner());
	if (!IsValid(PlayerController))
	{
		FGvDebug::Error(GetOwner()->GetName() + " Is not a player controller, ActionManagerComponent will not work");
		bInitialized = false;
		return;
	}
	bInitialized = true;

	SetupInputComponent();
}

void UActionManagerComponent::SetupInputComponent()
{
	CHECK_INITIALIZATION

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
		PlayerController->InputComponent))
	{
		TArray<FGameplayTag> InvalidTags = TArray<FGameplayTag>();
		for (const auto Element : PlayerActionMap)
		{
			if (!IsValid(Element.Value))
			{
				FGvDebug::Warning("Found invalid PlayerAction in Player Action Map");
				continue;
			}

			if (!Element.Key.IsValid() || Element.Key == FGameplayTag::EmptyTag)
			{
				FGvDebug::Warning("PlayerAction " + Element.Value->GetName() + " has invalid tag, the action will not work");
				InvalidTags.Add(Element.Key);
				continue;
			}
			
			if (Element.Value->Initialize(Element.Key))
			{
				EnhancedInputComponent->BindAction(Element.Value->GetInputAction(),
				                                   Element.Value->TriggerModeIsPressed()
					                                   ? ETriggerEvent::Started
					                                   : ETriggerEvent::Triggered, Element.Value,
				                                   &UPlayerAction::OnInputActionTriggered);
				EnhancedInputComponent->BindAction(Element.Value->GetInputAction(), ETriggerEvent::Completed,
				                                   Element.Value,
				                                   &UPlayerAction::OnInputActionCompleted);
			}
		}

		//Remove invalid tags found in the loop above
		for (auto InvalidTag : InvalidTags)
		{
			PlayerActionMap.Remove(InvalidTag);
		}
	}
}
