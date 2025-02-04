// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/DismembermentSystem/Dismemberer.h"
#include "Components/PoseableMeshComponent.h"

UDismemberer::UDismemberer(): SkeletalMeshComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDismemberer::Init(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	SkeletalMeshComponent = InSkeletalMeshComponent;
}

void UDismemberer::DismemberLimb(const FName& BoneName, const FVector Impulse)
{
	if (!Check())
		return;

	if (!DismemberablaBones.Contains(BoneName) || DismemberedBones.Contains(BoneName))
        return;
	
	SkeletalMeshComponent->HideBoneByName(BoneName, EPhysBodyOp::PBO_Term);
	SpawnDisemberedLimb(BoneName, Impulse);
}

void UDismemberer::DismemberRandomLimbs(int32 LimbsCount, const FVector Impulse)
{
	if (!Check())
		return;

	TArray<FName> AvailableBones = DismemberablaBones;
	AvailableBones.RemoveAll([this](const FName BoneName)
	{
		return DismemberedBones.Contains(BoneName);
	});

	LimbsCount = FMath::Clamp(LimbsCount, 0, AvailableBones.Num());
	for (int32 i = 0; i < LimbsCount; i++)
	{
		const FName RandomBoneName = AvailableBones[FMath::RandRange(0, AvailableBones.Num() - 1)];
		DismemberLimb(RandomBoneName, Impulse);
		AvailableBones.Remove(RandomBoneName);
	}
}

USkeletalMeshComponent* UDismemberer::GetSkeletalMeshComponent() const
{
	return SkeletalMeshComponent;
}

TArray<FName> UDismemberer::GetDismemberedBones() const
{
	return DismemberedBones;
}

void UDismemberer::BeginPlay()
{
	Super::BeginPlay();
}

void UDismemberer::SpawnDisemberedLimb(const FName BoneName, const FVector Impulse)
{
    if (!Check())
        return;

    ADismemberedLimb* DismemberedLimb = GetWorld()->SpawnActor<ADismemberedLimb>();
    DismemberedLimb->Init(this, BoneName, Impulse);
    DismemberedBones.Append(DismemberedLimb->GetLimbBoneNames());
}

bool UDismemberer::Check() const
{
	return IsValid(SkeletalMeshComponent);
}
