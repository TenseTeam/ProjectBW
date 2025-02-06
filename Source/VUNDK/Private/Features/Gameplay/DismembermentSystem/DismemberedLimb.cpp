// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/DismembermentSystem/DismemberedLimb.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Features/Gameplay/DismembermentSystem/Dismemberer.h"
#include "Features/Gameplay/DismembermentSystem/Utility/DismemberUtility.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/SkeletalBodySetup.h"
#include "PhysicsEngine/SphereElem.h"

class USkeletalBodySetup;

ADismemberedLimb::ADismemberedLimb()
{
	PrimaryActorTick.bCanEverTick = false;
	LimbRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	SetRootComponent(LimbRootComponent);
}

void ADismemberedLimb::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);

	UDismemberUtility::SpawnBloodDecal(
		GetWorld(),
		Dismemberer->GetRandomBloodStainDecal(),
		Data[0].Position,
		Dismemberer->BloodDecalSpawnParams
	);
}

void ADismemberedLimb::Init(UDismemberer* InDismemberer, const FName BoneName, const FVector Impulse)
{
	Dismemberer = InDismemberer;
	TargetSkelatalMeshComponent = Dismemberer->GetSkeletalMeshComponent();
	TargetBoneName = BoneName;

	if (!Check())
		return;

	PoseableMesh->SetSkinnedAssetAndUpdate(TargetSkelatalMeshComponent->GetSkeletalMeshAsset());
	SetActorLabel(TargetBoneName.ToString());

	IsolateLimb();
	SetLimbActorLocationAndRotation();
	SpawnBloodSpillFX();
	UPrimitiveComponent* CollisionComp = CreateApproximateCollision();
	CollisionComp->AddImpulse(Impulse, TargetBoneName);
}

TArray<FName> ADismemberedLimb::GetLimbBoneNames() const
{
	return LimbBoneNames;
}

float ADismemberedLimb::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (!Check())
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	SpawnBloodExplosionFX();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ADismemberedLimb::SpawnBloodExplosionFX()
{
	UDismemberUtility::SpawnBloodExplosionFX(GetWorld(), Dismemberer->GetRandomBloodExplosionFX(), GetActorLocation(), Dismemberer->NiagaraCallbackName, this);
}

void ADismemberedLimb::SpawnBloodSpillFX()
{
	if (Dismemberer->BloodSpillFXs.Num() == 0)
		return;

	const FVector BoneLocation = PoseableMesh->GetSocketLocation(TargetBoneName);
	const FVector PivotLocation = PoseableMesh->GetSocketLocation(Dismemberer->PivotOrientationName);
	const FVector SprayDirection = (PivotLocation - BoneLocation).GetSafeNormal();
	UDismemberUtility::SpawnBloodSpillFX(GetWorld(), Dismemberer->GetRandomBloodSpillFX(), PoseableMesh, TargetBoneName, SprayDirection, Dismemberer->NiagaraCallbackName, this);
}

void ADismemberedLimb::IsolateLimb()
{
	const FVector BoneLoc = PoseableMesh->GetBoneLocationByName(TargetBoneName, EBoneSpaces::ComponentSpace);

	LimbBoneNames.Add(TargetBoneName);
	for (int32 i = 1; i < PoseableMesh->GetNumBones(); i++) // Skip the root bone, set the location of all other bones to the location of the isolated bone
	{
		const FName CurrentBoneName = TargetSkelatalMeshComponent->GetBoneName(i);
		const FRotator CurrentBoneRot = TargetSkelatalMeshComponent->GetBoneQuaternion(CurrentBoneName, EBoneSpaces::ComponentSpace).Rotator();
		PoseableMesh->SetBoneRotationByName(CurrentBoneName, CurrentBoneRot, EBoneSpaces::ComponentSpace);

		if (PoseableMesh->BoneIsChildOf(CurrentBoneName, TargetBoneName))
			LimbBoneNames.Add(CurrentBoneName);
		else
			PoseableMesh->SetBoneLocationByName(CurrentBoneName, BoneLoc, EBoneSpaces::ComponentSpace);
	}

	PoseableMesh->SetBoneScaleByName(GetRootBoneName(), FVector::One() * BoneScale, EBoneSpaces::ComponentSpace);
	PoseableMesh->SetBoneScaleByName(TargetBoneName, FVector::One(), EBoneSpaces::WorldSpace);

	for (const FName& BoneName : Dismemberer->GetDismemberedBones())
		PoseableMesh->HideBoneByName(BoneName, EPhysBodyOp::PBO_Term);
}

void ADismemberedLimb::SetLimbActorLocationAndRotation()
{
	const FVector WorldLoc = TargetSkelatalMeshComponent->GetBoneLocation(TargetBoneName, EBoneSpaces::WorldSpace);
	const FRotator WorldRot = TargetSkelatalMeshComponent->GetComponentRotation();
	SetActorLocationAndRotation(WorldLoc, WorldRot);
}

FName ADismemberedLimb::GetRootBoneName() const
{
	return PoseableMesh->GetBoneName(0);
}

UPrimitiveComponent* ADismemberedLimb::CreateApproximateCollision()
{
	UPhysicsAsset* PhysicsAsset = TargetSkelatalMeshComponent->GetPhysicsAsset();
	if (!IsValid(PoseableMesh))
		return nullptr;

	UPrimitiveComponent* PrimitiveComponent = nullptr;
	for (const USkeletalBodySetup* BodySetup : PhysicsAsset->SkeletalBodySetups)
	{
		if (!GetLimbBoneNames().Contains(BodySetup->BoneName))
			continue;

		if (BodySetup->AggGeom.SphereElems.Num() > 0)
		{
			const FKSphereElem& SphereElem = BodySetup->AggGeom.SphereElems[0];
			UBoxComponent* BoxComponent = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass());
			BoxComponent->SetBoxExtent(FVector(SphereElem.Radius));
			PrimitiveComponent = BoxComponent;
			break;
		}

		if (BodySetup->AggGeom.SphylElems.Num() > 0)
		{
			const FKSphylElem& SphylElem = BodySetup->AggGeom.SphylElems[0];
			UBoxComponent* BoxComponent = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass());
			BoxComponent->SetBoxExtent(FVector(SphylElem.Radius, SphylElem.Radius, SphylElem.Length));
			PrimitiveComponent = BoxComponent;
			break;
		}

		if (BodySetup->AggGeom.BoxElems.Num() > 0)
		{
			const FKBoxElem& BoxElem = BodySetup->AggGeom.BoxElems[0];
			UBoxComponent* BoxComponent = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass());
			BoxComponent->SetBoxExtent(FVector(BoxElem.X, BoxElem.Y, BoxElem.Z));
			PrimitiveComponent = BoxComponent;
			break;
		}
	}

	if (!IsValid(PrimitiveComponent))
	{
		// If no collision primitive was found, create a capsule component
		UCapsuleComponent* CapsuleComponent = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass());
		CapsuleComponent->SetCapsuleSize(10.0f, 20.0f);
		PrimitiveComponent = CapsuleComponent;
	}

	PrimitiveComponent->SetupAttachment(RootComponent);
	PrimitiveComponent->RegisterComponent();
	AddInstanceComponent(PrimitiveComponent);
	PrimitiveComponent->Activate();

	PoseableMesh->AttachToComponent(PrimitiveComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PrimitiveComponent->SetCollisionProfileName(TEXT("Ragdoll"));
	PrimitiveComponent->SetSimulatePhysics(true);

	return PrimitiveComponent;
}

bool ADismemberedLimb::Check() const
{
	return IsValid(Dismemberer) && IsValid(TargetSkelatalMeshComponent);
}
