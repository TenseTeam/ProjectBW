// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/DismembermentSystem/Dismemberer.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Features/Gameplay/DismembermentSystem/Utility/DismemberUtility.h"

UDismemberer::UDismemberer(): SkeletalMeshComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDismemberer::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	if (BloodStainsDecals.Num() == 0)
		return;

	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);
	
	UDismemberUtility::SpawnBloodDecal(
		GetWorld(),
		GetRandomBloodStainDecal(),
		Data[0].Position,
		FVector(FMath::RandRange(BloodStainMinSize, BloodStainMaxSize)),
		BloodStainDuration,
		BloodStainChance
	);
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

	SpawnDisemberedLimb(BoneName, Impulse);
	SpawnDismemberFX(BoneName);
	SkeletalMeshComponent->HideBoneByName(BoneName, EPhysBodyOp::PBO_Term);
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

void UDismemberer::SpawnDismemberFX(const FName& BoneName)
{
	if (DismemberFXs.Num() == 0)
		return;

	const FName ParentBone = SkeletalMeshComponent->GetParentBone(BoneName);
	if (ParentBone.IsNone())
		return;

	const FVector BoneLocation = SkeletalMeshComponent->GetSocketLocation(BoneName);
	const FVector PivotLocation = SkeletalMeshComponent->GetSocketLocation(PivotOrientationName);
	const FVector SprayDirection = (BoneLocation - PivotLocation).GetSafeNormal();
	UNiagaraComponent* NiagaraComponent = UDismemberUtility::SpawnDismemberFX(GetRandomDismemberFX(), SkeletalMeshComponent, ParentBone, SprayDirection);
	NiagaraComponent->SetVariableObject(NiagaraCallbackName, this);
}

UNiagaraSystem* UDismemberer::GetRandomDismemberFX() const
{
	return DismemberFXs[FMath::RandRange(0, DismemberFXs.Num() - 1)];
}

UMaterialInterface* UDismemberer::GetRandomBloodStainDecal() const
{
	return BloodStainsDecals[FMath::RandRange(0, BloodStainsDecals.Num() - 1)];
}

void UDismemberer::SpawnDisemberedLimb(const FName BoneName, const FVector Impulse)
{
	if (!Check())
		return;

	ADismemberedLimb* DismemberedLimb = GetWorld()->SpawnActor<ADismemberedLimb>();
	DismemberedLimb->Init(this, BoneName, Impulse, GetRandomDismemberFX());
	DismemberedBones.Append(DismemberedLimb->GetLimbBoneNames());
}

bool UDismemberer::Check() const
{
	return IsValid(SkeletalMeshComponent) && IsValid(GetWorld()) && DismemberablaBones.Num() > 0;
}
