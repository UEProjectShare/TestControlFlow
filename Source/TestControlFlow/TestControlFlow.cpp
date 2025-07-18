// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestControlFlow.h"
#include "ControlFlowBranch.h"
#include "ControlFlowConcurrency.h"
#include "ControlFlowConditionalLoop.h"
#include "Modules/ModuleManager.h"
#include "ControlFlowManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, TestControlFlow, "TestControlFlow" );

// Sets default values
ATestControlFlows::ATestControlFlows()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATestControlFlows::TestQueueStep()
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("TestQueueStep"))
	.QueueStep(TEXT("ConstructNode"), this, &ThisClass::Construct)
	.QueueStep(TEXT("QueueStepFunction"), this, &ThisClass::QueueStepFunction)
	.QueueStep(TEXT("DestructNode"), this, &ThisClass::Destruct);

	Flow.ExecuteFlow();
}

void ATestControlFlows::QueueStepFunction()
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueStepFunction"));
}

void ATestControlFlows::TestQueueWait()
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("TestQueueWait"))
	.QueueStep(TEXT("ConstructNode"), this, &ThisClass::Construct)
	.QueueStep(TEXT("QueueWaitFunction"), this, &ThisClass::QueueWaitFunction)
	.QueueStep(TEXT("DestructNode"), this, &ThisClass::Destruct);

	Flow.ExecuteFlow();
}

void ATestControlFlows::QueueWaitFunction(FControlFlowNodeRef SubFlow)
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueWaitFunction"));

	int32 Count = 0;
	while (Count <= 10)
	{
		UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueWaitFunction Count: %d"), Count);
		
		Count++;
		FPlatformProcess::Sleep(.5f);
	}

	SubFlow->ContinueFlow();
}

void ATestControlFlows::TestQueueControlFlow()
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("TestQueueControlFlow"))
	.QueueStep(TEXT("ConstructNode"), this, &ThisClass::Construct)
	.QueueStep(TEXT("QueueControlFlow"), this, &ThisClass::QueueControlFlow, 0, FString("TestQueueControlFlow"))
	.QueueStep(TEXT("DestructNode"), this, &ThisClass::Destruct);

	Flow.ExecuteFlow();
}

void ATestControlFlows::QueueControlFlow(TSharedRef<FControlFlow> FlowRef, int32 Param1, FString Param2)
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueControlFlow Param1:%d, Param2:%s"), Param1, *Param2);
}

void ATestControlFlows::TestQueueControlFlowBranch()
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("TestQueueControlFlowBranch"))
	.QueueStep(TEXT("ConstructNode"), this, &ThisClass::Construct)
	.BranchFlow([this](TSharedRef<FControlFlowBranch> Branch)
	{
		Branch->AddOrGetBranch(1).QueueStep(TEXT("QueueControlFlowBranch1"), this, &ThisClass::QueueControlFlowBranch1, 1, FString("TestQueueControlFlowBranch1"));
		Branch->AddOrGetBranch(2).QueueStep(TEXT("QueueControlFlowBranch2"), this, &ThisClass::QueueControlFlowBranch2, 2, FString("TestQueueControlFlowBranch2"));
		
		return FMath::RandBool() ? 1 : 2;
	})
	.QueueStep(TEXT("DestructNode"), this, &ThisClass::Destruct);
	
	Flow.ExecuteFlow();
}

void ATestControlFlows::QueueControlFlowBranch1(int32 Param1, FString Param2)
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueControlFlowBranch1 Param1:%d, Param2:%s"), Param1, *Param2);
}

void ATestControlFlows::QueueControlFlowBranch2(int32 Param1, FString Param2)
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueControlFlowBranch2 Param1:%d, Param2:%s"), Param1, *Param2);
}

void ATestControlFlows::TestQueueConcurrentFlows()
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("TestQueueControlFlowBranch"))
	.QueueStep(TEXT("ConstructNode"), this, &ThisClass::Construct)
	.ForkFlow([this](TSharedRef<FConcurrentControlFlows> ConcurrentFlows)
	{
		ConcurrentFlows->AddOrGetFlow(0).QueueStep(TEXT("QueueConcurrentFlows1"), this, &ThisClass::QueueConcurrentFlows1, 1, FString("QueueConcurrentFlows1"));
		ConcurrentFlows->AddOrGetFlow(1).QueueStep(TEXT("QueueConcurrentFlows2"), this, &ThisClass::QueueConcurrentFlows2, 2, FString("QueueConcurrentFlows2"));
	})
	.QueueStep(TEXT("DestructNode"), this, &ThisClass::Destruct);
	
	Flow.ExecuteFlow();
}

void ATestControlFlows::QueueConcurrentFlows1(int32 Param1, FString Param2)
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueConcurrentFlows1 Param1:%d, Param2:%s"), Param1, *Param2);
}

void ATestControlFlows::QueueConcurrentFlows2(int32 Param1, FString Param2)
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueConcurrentFlows2 Param1:%d, Param2:%s"), Param1, *Param2);
}

void ATestControlFlows::TestQueueConditionalLoop()
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("TestQueueConditionalLoop"))
	.QueueStep(TEXT("ConstructNode"), this, &ThisClass::Construct)
	//do while
	.Loop([this](TSharedRef<FConditionalLoop> InnerLoop)
	{
		UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueConditionalLoop Loop1"));
	
		InnerLoop->RunLoopFirst().QueueStep(TEXT("QueueConditionalLoop"), this, &ThisClass::QueueConditionalLoop, 1, FString("QueueConditionalLoop"));
		
		return LoopCounter <= 10 ? EConditionalLoopResult::RunLoop : EConditionalLoopResult::LoopFinished;
	})
	//while
	.Loop([this](TSharedRef<FConditionalLoop> InnerLoop)
	{
		UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueConditionalLoop Loop2"));
		
		InnerLoop->CheckConditionFirst().QueueStep(TEXT("QueueConditionalLoop"), this, &ThisClass::QueueConditionalLoop, 1, FString("QueueConditionalLoop"));
		
		return LoopCounter <= 10 ? EConditionalLoopResult::RunLoop : EConditionalLoopResult::LoopFinished;
	})
	.QueueStep(TEXT("DestructNode"), this, &ThisClass::Destruct);
	
	Flow.ExecuteFlow();
}

void ATestControlFlows::QueueConditionalLoop(int32 Param1, FString Param2)
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueConditionalLoop Param1:%d, Param2:%s"), Param1, *Param2);

	LoopCounter++;
	
	FPlatformProcess::Sleep(.5f);

	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::QueueConditionalLoop LoopCounter:%d"), LoopCounter);
}

void ATestControlFlows::Construct()
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::Construct"));
}

void ATestControlFlows::Destruct()
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::Destruct"));
}
