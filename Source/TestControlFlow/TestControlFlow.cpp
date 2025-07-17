// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestControlFlow.h"
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

void ATestControlFlows::Construct()
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::Construct"));
}

void ATestControlFlows::Destruct()
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::Destruct"));
}

