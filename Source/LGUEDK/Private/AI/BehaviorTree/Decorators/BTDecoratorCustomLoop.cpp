// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecoratorCustomLoop.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"
#include "BehaviorTree/Decorators/BTDecorator_Loop.h"

UBTDecoratorCustomLoop::UBTDecoratorCustomLoop(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "RandomLoop";
	NumLoopsMin = 1;
	NumLoopsMax = 2;
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
 
	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
	
}
 
void UBTDecoratorCustomLoop::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	FBTLoopDecoratorMemory* DecoratorMemory = GetNodeMemory<FBTLoopDecoratorMemory>(SearchData);
	FBTCompositeMemory* ParentMemory = GetParentNode()->GetNodeMemory<FBTCompositeMemory>(SearchData);
	const bool bIsSpecialNode = GetParentNode()->IsA(UBTComposite_SimpleParallel::StaticClass());
 
	if ((bIsSpecialNode && ParentMemory->CurrentChild == BTSpecialChild::NotInitialized) ||
		(!bIsSpecialNode && ParentMemory->CurrentChild != ChildIndex))
	{
		int32 numLoops = FMath::RandRange(NumLoopsMin, NumLoopsMax);
		DecoratorMemory->RemainingExecutions = IntCastChecked<uint8>(numLoops);
		DecoratorMemory->TimeStarted = GetWorld()->GetTimeSeconds();
	}
 
	bool bShouldLoop = false;
	if (DecoratorMemory->RemainingExecutions > 0)
	{
		DecoratorMemory->RemainingExecutions--;
	}
	bShouldLoop = DecoratorMemory->RemainingExecutions > 0;
 

	if (bShouldLoop)
	{
		GetParentNode()->SetChildOverride(SearchData, ChildIndex);
	}
}
 
FString UBTDecoratorCustomLoop::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %d-%d loops"), *Super::GetStaticDescription(), NumLoopsMin, NumLoopsMax);
}
 
void UBTDecoratorCustomLoop::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
 
	FBTLoopDecoratorMemory* DecoratorMemory = (FBTLoopDecoratorMemory*)NodeMemory;
	Values.Add(FString::Printf(TEXT("loops remaining: %d"), DecoratorMemory->RemainingExecutions));
}
 
uint16 UBTDecoratorCustomLoop::GetInstanceMemorySize() const
{
	return sizeof(FBTLoopDecoratorMemory);
}
 
void UBTDecoratorCustomLoop::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FBTLoopDecoratorMemory>(NodeMemory, InitType);
}
 
void UBTDecoratorCustomLoop::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FBTLoopDecoratorMemory>(NodeMemory, CleanupType);
}
 
#if WITH_EDITOR
 
FName UBTDecoratorCustomLoop::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Decorator.Loop.Icon");
}
 
#endif	