// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/DismembermentSystem/Utility/DismemberUtility.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void UDismemberUtility::SpawnBloodDecal(const UWorld* World, UMaterialInterface* DecalMaterial, const FVector& ParticleLocation, const FVector& Size, const float BloodStainDuration, const float DecalSpawnChance = 1.f)
{
	if (!IsValid(World) || !IsValid(DecalMaterial))
		return;

	if (FMath::RandRange(0.f, 1.f) > DecalSpawnChance)
		return;
	
	TArray<FHitResult> HitResults;
	const FCollisionQueryParams CollisionParams;
	if (World->SweepMultiByChannel(HitResults, ParticleLocation, ParticleLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Size.X / 2.f), CollisionParams))
	{
		FVector AverageNormal = FVector::ZeroVector;

		for (const FHitResult& HitResult : HitResults)
		{
			const APawn* HitPawn = nullptr;
			HitPawn = HitResult.GetActor()->IsA<APawn>() ? Cast<APawn>(HitResult.GetActor()) : nullptr;
			if (IsValid(HitPawn))
				UGameplayStatics::SpawnDecalAttached(DecalMaterial, Size / 2.f, HitResult.Component.Get(), HitResult.BoneName, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), EAttachLocation::Type::KeepWorldPosition, BloodStainDuration);

			AverageNormal += HitResult.ImpactNormal;
		}

		AverageNormal /= HitResults.Num();
		AverageNormal.Normalize();

		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(World, DecalMaterial, Size, ParticleLocation, AverageNormal.Rotation(), BloodStainDuration);
		Decal->SetSortOrder(FMath::RandRange(-100, 100));
	}
}

UNiagaraComponent* UDismemberUtility::SpawnDismemberFX(UNiagaraSystem* DismemberFX, USceneComponent* AttachToComponent, const FName& AttachToBoneName, const FVector& SprayDirection)
{
	if (!IsValid(DismemberFX) || !IsValid(AttachToComponent))
		return nullptr;
	
	FRotator SprayRotation = SprayDirection.ToOrientationRotator();
	SprayRotation.Pitch -= 90.0f;

	FFXSystemSpawnParameters SpawnParams;
	SpawnParams.SystemTemplate = DismemberFX;
	SpawnParams.AttachToComponent = AttachToComponent;
	SpawnParams.AttachPointName = AttachToBoneName;
	SpawnParams.PoolingMethod = EPSCPoolMethod::AutoRelease;
	SpawnParams.bAutoActivate = true;
	SpawnParams.bAutoDestroy = true;
	SpawnParams.bPreCullCheck = true;
	SpawnParams.LocationType = EAttachLocation::Type::SnapToTargetIncludingScale;

	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttachedWithParams(SpawnParams);
	NiagaraComponent->SetWorldRotation(SprayRotation);
	return NiagaraComponent;
}
