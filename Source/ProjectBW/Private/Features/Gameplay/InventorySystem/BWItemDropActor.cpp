// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/InventorySystem/BWItemDropActor.h"

#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventory.h"
#include "Features/Player/BWCharacter.h"
#include "Kismet/GameplayStatics.h"


void ABWItemDropActor::BeginPlay()
{
	Super::BeginPlay();
	PlayerInventory = Cast<URPGInventory>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetComponentByClass(URPGInventory::StaticClass()));
}

void ABWItemDropActor::Interact_Implementation()
{
	if (IsValid(PlayerInventory))
	{
		PlayerInventory->TryAddItem(RelatedItem);
		Destroy();
	}
}

bool ABWItemDropActor::CanBeInteracted_Implementation(AActor* Caller) const
{
	FVector EndTraceLocation;
	ABWCharacter* BWCharacter = Cast<ABWCharacter>(Caller);
	if (IsValid(BWCharacter))
	{
		FMinimalViewInfo CameraViewInfo;
		BWCharacter->GetFollowCamera()->GetCameraView(GetWorld()->GetDeltaSeconds(), CameraViewInfo);
		EndTraceLocation = CameraViewInfo.Location;
	}
	else EndTraceLocation = Caller->GetActorLocation();
	
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), EndTraceLocation, ECC_Visibility);
	return !HitResult.bBlockingHit;
}

FVector ABWItemDropActor::GetInteractableLocation_Implementation() const
{
	return GetActorLocation();
}
