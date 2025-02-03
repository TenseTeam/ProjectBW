// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/DismembermentSystem/DismemberedLimb.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Features/Gameplay/DismembermentSystem/Dismemberer.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/SkeletalBodySetup.h"

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

	UShapeComponent* ShapeComponent = CreateCollisions();
	PoseableMesh->AttachToComponent(ShapeComponent, FAttachmentTransformRules::KeepRelativeTransform);
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

UShapeComponent* ADismemberedLimb::CreateCollisions()
{
	UShapeComponent* AttachComponent = CapsuleComponent;
	UPhysicsAsset* PhysicsAsset = TargetSkelatalMeshComponent->GetPhysicsAsset();

	if (!IsValid(PhysicsAsset))
		return AttachComponent;

	for (const USkeletalBodySetup* BodySetup : PhysicsAsset->SkeletalBodySetups)
	{
		if (!GetLimbBoneNames().Contains(BodySetup->BoneName))
			continue;

		const FTransform BoneTransform = TargetSkelatalMeshComponent->GetBoneTransform(BodySetup->BoneName);
		
		for (const FKSphereElem& SphereElem : BodySetup->AggGeom.SphereElems)
		{
			USphereComponent* SphereCollision = NewObject<USphereComponent>(this);
			SphereCollision->AttachToComponent(AttachComponent, FAttachmentTransformRules::KeepRelativeTransform);
		
			SphereCollision->SetSphereRadius(SphereElem.Radius);
			// SphereCollision->SetRelativeTransform(SphereElem.GetTransform() * BoneTransform.Inverse());
			
			SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SphereCollision->SetCollisionProfileName(TEXT("Ragdoll"));
			SphereCollision->SetHiddenInGame(false);
			SphereCollision->SetSimulatePhysics(true);
			SphereCollision->RegisterComponent();
		
			AttachComponent = SphereCollision;
		}
		
		for (const FKSphylElem& SphylElem : BodySetup->AggGeom.SphylElems)
		{
			UCapsuleComponent* CapsuleCollision = NewObject<UCapsuleComponent>(this);
			CapsuleCollision->AttachToComponent(AttachComponent, FAttachmentTransformRules::KeepRelativeTransform);
		
			CapsuleCollision->SetCapsuleSize(SphylElem.Radius, SphylElem.Length);
			// CapsuleCollision->SetRelativeTransform(SphylElem.GetTransform() * BoneTransform.Inverse());
		
			CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CapsuleCollision->SetCollisionProfileName(TEXT("Ragdoll"));
			CapsuleCollision->SetHiddenInGame(false);
			CapsuleCollision->SetSimulatePhysics(true);
			CapsuleCollision->RegisterComponent();
		
			AttachComponent = CapsuleComponent;
		}
		
		for (const FKSphylElem& SphylElem : BodySetup->AggGeom.SphylElems)
		{
			UBoxComponent* BoxCollision = NewObject<UBoxComponent>(this);
			BoxCollision->AttachToComponent(AttachComponent, FAttachmentTransformRules::KeepRelativeTransform);
		
			BoxCollision->SetBoxExtent(FVector(SphylElem.Radius, SphylElem.Radius, SphylElem.Length));
			// BoxCollision->SetRelativeTransform(SphylElem.GetTransform() * BoneTransform.Inverse());
			//
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			BoxCollision->SetCollisionProfileName(TEXT("Ragdoll"));
			BoxCollision->SetHiddenInGame(false);
			BoxCollision->SetSimulatePhysics(true);
			BoxCollision->RegisterComponent();
		
			AttachComponent = BoxCollision;
		}
	}


	return AttachComponent;
}

bool ADismemberedLimb::Check() const
{
	return IsValid(Dismemberer) && IsValid(TargetSkelatalMeshComponent);
}
