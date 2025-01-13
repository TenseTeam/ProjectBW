// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/InteractionSystem/Behaviours/SphereDetectionBehaviour.h"

#include <string>

#include "Engine/OverlapResult.h"
#include "Features/Gameplay/InteractionSystem/Interfaces/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/FGvDebug.h"


bool USphereDetectionBehaviour::TryGetInteractable(TScriptInterface<IInteractable>& OutInteractable) const
{
	TArray<FOverlapResult> Overlaps;
	//Check if there are any actors that match the trace channel and are within the max detection distance
	if (GetWorld()->OverlapMultiByChannel(Overlaps, Owner->GetActorLocation(), FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(MaxDetectionDistance)))
	{

#if WITH_EDITOR && !UE_BUILD_SHIPPING
		if (bShowDebug)
			DrawDebugSphere(GetWorld(), Owner->GetActorLocation(), MaxDetectionDistance, 12, FColor::Green, false, -1.f);
#endif
		
		//Check if the overlapped actors are interactable
		TArray<TScriptInterface<IInteractable>> Interactables = TArray<TScriptInterface<IInteractable>>();
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UInteractable>())
			{
				TScriptInterface<IInteractable> Interactable = TScriptInterface<IInteractable>(Overlap.GetActor());
				if (Interactable->Execute_CanBeInteracted(Overlap.GetActor(), Owner))
				{
					Interactables.Add(Interactable);
				}
			}
		}

		//FGvDebug::Log(std::to_string(Interactables.Num()).c_str(), true);
		
		//If there are more than one interactable, get the most relevant
		if (Interactables.Num() > 1)
		{
			OutInteractable = GetMostRelevantInteractable(Interactables);
			return OutInteractable != nullptr;
		}
		//If there is only one interactable, return it
		if (Interactables.Num() == 1)
		{
			OutInteractable = Interactables[0];
			return true;
		}
	}
	//If there are no interactables, return nullptr
	OutInteractable = nullptr;
	return false;
}


TScriptInterface<IInteractable> USphereDetectionBehaviour::GetMostRelevantInteractable(TArray<TScriptInterface<IInteractable>> Interactables) const
{
	TScriptInterface<IInteractable> MostRelevantInteractable = nullptr;
	float CoincidetAngle = 0.5f;
	const FVector CameraForward = Cast<APawn>(Owner)->GetControlRotation().Vector();
	
	for (auto Element : Interactables)
	{
		FVector Direction = (Element->Execute_GetInteractableLocation(Element.GetObject()) - GetOwnerCameraLocation()).GetSafeNormal();
		float Dot = FVector::DotProduct(Direction, CameraForward);
		FGvDebug::Log(std::to_string(Dot).c_str(), true);
		DrawDebugLine(Owner->GetWorld(), GetOwnerCameraLocation(), Element->Execute_GetInteractableLocation(Element.GetObject()), FColor::Red, false, -1.f);
		DrawDebugLine(Owner->GetWorld(), GetOwnerCameraLocation(), GetOwnerCameraLocation() + CameraForward * 300, FColor::Green, false, -1.f);
		if (Dot >= CoincidetAngle)
		{
			CoincidetAngle = Dot;
			MostRelevantInteractable = Element;
		}
	}
	return MostRelevantInteractable;
}


FVector USphereDetectionBehaviour::GetOwnerCameraLocation() const
{
	return UGameplayStatics::GetPlayerCameraManager(Owner, 0)->GetCameraCacheView().Location;
}
