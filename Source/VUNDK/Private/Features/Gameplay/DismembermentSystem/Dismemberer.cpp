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

void UDismemberer::DismemberLimb(const FName& BoneName)
{
	if (!Check())
		return;

	SkeletalMeshComponent->HideBoneByName(BoneName, EPhysBodyOp::PBO_Term);
	SpawnDisemberedLimb(BoneName);
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

void UDismemberer::SpawnDisemberedLimb(const FName BoneName)
{
	if (!Check())
		return;

	ADismemberedLimb* DismemberedLimb = GetWorld()->SpawnActor<ADismemberedLimb>();
	DismemberedLimb->Init(this, BoneName);
	DismemberedBones.Append(DismemberedLimb->GetLimbBoneNames());
}

bool UDismemberer::Check() const
{
	return IsValid(SkeletalMeshComponent);
}
