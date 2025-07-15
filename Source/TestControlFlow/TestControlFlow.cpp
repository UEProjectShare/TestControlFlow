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
		.QueueStep(TEXT("Foo"), this, &ThisClass::Foo)
		.QueueStep(TEXT("DestructNode"), this, &ThisClass::Destruct);

	Flow.ExecuteFlow();
}

void ATestControlFlows::Foo()
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::Foo"));
}

void ATestControlFlows::Construct()
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::Construct"));
}

void ATestControlFlows::Destruct()
{
	UE_LOG(LogTemp, Display, TEXT("ATestControlFlows::Destruct"));
}

