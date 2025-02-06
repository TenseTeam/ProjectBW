// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/DismembermentSystem/Utility/DismemberUtility.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void UDismemberUtility::SpawnBloodDecal(const UWorld* World, UMaterialInterface* DecalMaterial, const FVector& ParticleLocation, const FBloodDecalSpawnParams& BloodDecalSpawnParams)
{
	if (!IsValid(World) || !IsValid(DecalMaterial))
		return;

	if (FMath::RandRange(0.f, 1.f) > BloodDecalSpawnParams.SpawnChance)
		return;

	TArray<FHitResult> HitResults;
	const FCollisionQueryParams CollisionParams;
	const FVector Size = FVector(FMath::RandRange(BloodDecalSpawnParams.MinSize, BloodDecalSpawnParams.MaxSize));
	if (World->SweepMultiByChannel(HitResults, ParticleLocation, ParticleLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Size.X / 2.f), CollisionParams))
	{
		FVector AverageNormal = FVector::ZeroVector;

		for (const FHitResult& HitResult : HitResults)
		{
			const APawn* HitPawn = nullptr;
			HitPawn = HitResult.GetActor()->IsA<APawn>() ? Cast<APawn>(HitResult.GetActor()) : nullptr;
			if (IsValid(HitPawn))
			{
				UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(DecalMaterial, Size / 2.f, HitResult.Component.Get(), HitResult.BoneName, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), EAttachLocation::Type::KeepWorldPosition, BloodDecalSpawnParams.DecalDuration);
				Decal->bUseAttachParentBound = true;
			}
			AverageNormal += HitResult.ImpactNormal;
		}

		AverageNormal /= HitResults.Num();
		AverageNormal.Normalize();

		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(World, DecalMaterial, Size, ParticleLocation, AverageNormal.Rotation(), BloodDecalSpawnParams.DecalDuration);
		Decal->SetSortOrder(FMath::RandRange(BloodDecalSpawnParams.MinSortOrder, BloodDecalSpawnParams.MaxSortOrder));
		Decal->bUseAttachParentBound = true;
	}
}

UNiagaraComponent* UDismemberUtility::SpawnBloodExplosionFX(const UWorld* World, UNiagaraSystem* BloodExplosionFX, const FVector& ExplosionLocation, const FName& NiagaraCallbackName, UObject* CallbackObject)
{
	if (!IsValid(World) || !IsValid(BloodExplosionFX))
		return nullptr;

	FFXSystemSpawnParameters SpawnParams;
	SpawnParams.WorldContextObject = World;
	SpawnParams.SystemTemplate = BloodExplosionFX;
	SpawnParams.Location = ExplosionLocation;
	SpawnParams.PoolingMethod = EPSCPoolMethod::AutoRelease;
	SpawnParams.bAutoActivate = true;
	SpawnParams.bAutoDestroy = true;
	SpawnParams.bPreCullCheck = true;
	SpawnParams.LocationType = EAttachLocation::Type::KeepWorldPosition;

	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocationWithParams(SpawnParams);
	NiagaraComponent->SetVariableObject(NiagaraCallbackName, CallbackObject);
	return NiagaraComponent;
}

UNiagaraComponent* UDismemberUtility::SpawnBloodSpillFX(const UWorld* World, UNiagaraSystem* BloodSpillFX, USceneComponent* AttachToComponent, const FName& AttachToBoneName, const FVector& SprayDirection, const FName& NiagaraCallbackName, UObject* CallbackObject)
{
	if (!IsValid(World) || !IsValid(BloodSpillFX) || !IsValid(AttachToComponent))
		return nullptr;

	FRotator SprayRotation = SprayDirection.ToOrientationRotator();
	SprayRotation.Pitch -= 90.0f;

	FFXSystemSpawnParameters SpawnParams;
	SpawnParams.WorldContextObject = World;
	SpawnParams.SystemTemplate = BloodSpillFX;
	SpawnParams.AttachToComponent = AttachToComponent;
	SpawnParams.AttachPointName = AttachToBoneName;
	SpawnParams.PoolingMethod = EPSCPoolMethod::AutoRelease;
	SpawnParams.bAutoActivate = true;
	SpawnParams.bAutoDestroy = true;
	SpawnParams.bPreCullCheck = true;
	SpawnParams.LocationType = EAttachLocation::Type::SnapToTargetIncludingScale;

	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttachedWithParams(SpawnParams);
	NiagaraComponent->SetWorldRotation(SprayRotation);
	NiagaraComponent->SetVariableObject(NiagaraCallbackName, CallbackObject);
	return NiagaraComponent;
}
