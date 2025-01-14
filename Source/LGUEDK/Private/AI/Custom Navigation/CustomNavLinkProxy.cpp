// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Custom Navigation/CustomNavLinkProxy.h"

#include "AIController.h"
#include "AI/NPC/BWNPCBaseEnemy/BWNPCBaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/LGDebug.h"


// Sets default values
ACustomNavLinkProxy::ACustomNavLinkProxy()
{
	PrimaryActorTick.bCanEverTick = true;
	bSmartLinkIsRelevant = true;
}

void ACustomNavLinkProxy::BeginPlay()
{
	Super::BeginPlay();

	//OnSmartLinkReached.AddDynamic(this, &ACustomNavLinkProxy::OnMySmartLinkReached);
	
	UNavLinkCustomComponent* NavLinkCustomComponent =  GetSmartLinkComp();
	FVector LeftPt = FVector(70, 0, 0);
	FVector RightPt = FVector(-70, 0, 0);
	ENavLinkDirection::Type Direction;
	NavLinkCustomComponent->GetLinkData(LeftPt, RightPt, Direction);
	Direction = AINavLinkDirection;
	NavLinkCustomComponent->SetLinkData(LeftPt, RightPt, Direction);

	//JumpHeightRequired = FMath::Abs(LeftPt.Z - RightPt.Z); 
}
//
// bool ACustomNavLinkProxy::IsAvailable() const
// {
//     return !bIsOccupied;
// }
//
// void ACustomNavLinkProxy::MarkAsOccupied(float OccupyDuration)
// {
//     if (bIsOccupied)
//     {
//         return; // If already occupied, do nothing
//     }
//
//     // Mark the link as occupied
//     bIsOccupied = true;
//
//     // Set a timer to release the Nav Link after a given duration
//     GetWorldTimerManager().SetTimer(
//         OccupyTimerHandle,
//         this,
//         &ACustomNavLinkProxy::Release,
//         OccupyDuration,
//         false
//     );
// }
//
// void ACustomNavLinkProxy::Release()
// {
//     // Release the Nav Link (make it available again)
//     bIsOccupied = false;
//     GetWorldTimerManager().ClearTimer(OccupyTimerHandle);
// }
//
// float ACustomNavLinkProxy::GetJumpHeightRequired() const
// {
//     return JumpHeightRequired;
// }
//
// void ACustomNavLinkProxy::OnMySmartLinkReached(AActor* Agent, const FVector& Destination)
// {
//     // Log message when an agent reaches the smart link (you can replace this with custom logic)
//     UE_LOG(LogTemp, Warning, TEXT("Smart Link Reached"));
//
//     // Example: Check if the agent can jump or if it's available
//     if (IsAvailable())
//     {
//             AActor* ControlledPawn = Agent->GetOwner();
//             ABWNPCBaseEnemy* NPC = Cast<ABWNPCBaseEnemy>(ControlledPawn);
//             if (ControlledPawn)
//             {
//                 // Calculate jump height and check if the agent can jump over the NavLink
//                 if (JumpHeightRequired <= NPC->GetJumpHeight())
//                 {
//                     // If the jump height is enough, perform the jump
//                     FVector JumpDirection = (Destination - Agent->GetActorLocation()).GetSafeNormal();
//                     FVector LaunchVelocity = JumpDirection * 600.0f; 
//                     LaunchVelocity.Z = FMath::Sqrt(2 * JumpHeightRequired * FMath::Abs(GetWorld()->GetGravityZ())); // Calculate vertical speed
//
//                     if (ACharacter* Character = Cast<ACharacter>(ControlledPawn))
//                     {
//                         Character->LaunchCharacter(LaunchVelocity, true, true);
//                     }
//                 }
//                 else
//                 {
//                     // If jump height is insufficient, let the agent continue path following
//                     ResumePathFollowing(Agent);
//                 }
//             }
//         }
//            
//     
// }
