// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/DismembermentSystem/Dismemberer.h"
#include "NiagaraFunctionLibrary.h"
#include "Features/Gameplay/DismembermentSystem/Utility/DismemberUtility.h"

UDismemberer::UDismemberer(): BloodDecalSpawnParams(),
                              SkeletalMeshComponent(nullptr)
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
		BloodDecalSpawnParams
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
	SpawnBloodSpillFX(BoneName);
	SpawnBloodExplosionFX(SkeletalMeshComponent->GetSocketLocation(BoneName));
	SkeletalMeshComponent->HideBoneByName(BoneName, EPhysBodyOp::PBO_Term);
}

void UDismemberer::DismemberLimbs(const TArray<FName>& BoneNames, const FVector Impulse)
{
	if (!Check())
		return;

	for (const FName BoneName : BoneNames)
		DismemberLimb(BoneName, Impulse);
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

void UDismemberer::DismemberAllLimbs(const FVector Impulse)
{
	if (!Check())
		return;

	for (const FName BoneName : DismemberablaBones)
		DismemberLimb(BoneName, Impulse);
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
	GetOwner()->OnTakePointDamage.AddDynamic(this, &UDismemberer::OnTakePointDamage);
}

void UDismemberer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetOwner()->OnTakePointDamage.RemoveDynamic(this, &UDismemberer::OnTakePointDamage);
}

void UDismemberer::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	if (!Check())
		return;
	
	SpawnBloodExplosionFX(HitLocation);
}

void UDismemberer::SpawnBloodExplosionFX(const FVector& HitLocation)
{
	UDismemberUtility::SpawnBloodExplosionFX(GetWorld(), GetRandomBloodExplosionFX(), HitLocation, NiagaraCallbackName, this);
}

void UDismemberer::SpawnBloodSpillFX(const FName& BoneName)
{
	if (BloodSpillFXs.Num() == 0)
		return;

	const FName ParentBone = SkeletalMeshComponent->GetParentBone(BoneName);
	if (ParentBone.IsNone())
		return;

	const FVector BoneLocation = SkeletalMeshComponent->GetSocketLocation(BoneName);
	const FVector PivotLocation = SkeletalMeshComponent->GetSocketLocation(PivotOrientationName);
	const FVector SprayDirection = (BoneLocation - PivotLocation).GetSafeNormal();
	UDismemberUtility::SpawnBloodSpillFX(GetWorld(), GetRandomBloodSpillFX(), SkeletalMeshComponent, ParentBone, SprayDirection, NiagaraCallbackName, this);
}

UNiagaraSystem* UDismemberer::GetRandomBloodSpillFX() const
{
	return BloodSpillFXs[FMath::RandRange(0, BloodSpillFXs.Num() - 1)];
}

UNiagaraSystem* UDismemberer::GetRandomBloodExplosionFX() const
{
	return BloodExplosionFXs[FMath::RandRange(0, BloodSpillFXs.Num() - 1)];
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
	DismemberedLimb->Init(this, BoneName, Impulse);
	DismemberedBones.Append(DismemberedLimb->GetLimbBoneNames());
}

bool UDismemberer::Check() const
{
	return IsValid(SkeletalMeshComponent) && IsValid(GetWorld()) && DismemberablaBones.Num() > 0;
}
