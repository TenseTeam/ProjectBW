// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/DismembermentSystem/DismemberedLimb.h"
#include "Features/Gameplay/DismembermentSystem/Dismemberer.h"

ADismemberedLimb::ADismemberedLimb()
{
	PrimaryActorTick.bCanEverTick = false;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	SetRootComponent(CapsuleComponent);
	PoseableMesh->SetupAttachment(CapsuleComponent);
}

void ADismemberedLimb::Init(UDismemberer* InDismemberer, const FName BoneName)
{
	Dismemberer = InDismemberer;
	TargetSkelatalMeshComponent = Dismemberer->GetSkeletalMeshComponent();
	TargetBoneName = BoneName;

	PoseableMesh->SetSkinnedAssetAndUpdate(TargetSkelatalMeshComponent->GetSkeletalMeshAsset());

	IsolateLimb();
	SetLimbActorLocationAndRotation();

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionProfileName(TEXT("Ragdoll"));
	CapsuleComponent->SetSimulatePhysics(true);

	const FTransform Trans = TargetSkelatalMeshComponent->GetBoneTransform(TargetBoneName, ERelativeTransformSpace::RTS_Component);


	// FBodyInstance* BodyInstance = TargetSkelatalMeshComponent->GetBodyInstance(BoneName);
	// BodyInstance->BodySetup->AggGeom.GetBox(0).GetCenter();
	// TODO: Add collisions for each type of bone

	const FString Str = Trans.ToHumanReadableString();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bone Transform: %s"), *Str));

	// PoseableMesh->SetBoneScaleByName()
	// const FTransform Trans = PoseableMesh->GetBoneTransformByName(TargetBoneName, EBoneSpaces::Type::WorldSpace);
}

TArray<FName> ADismemberedLimb::GetLimbBoneNames() const
{
	return LimbBoneNames;
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

bool ADismemberedLimb::Check() const
{
	return IsValid(Dismemberer) && IsValid(TargetSkelatalMeshComponent);
}
