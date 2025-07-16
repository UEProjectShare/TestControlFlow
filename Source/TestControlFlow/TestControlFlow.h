// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ControlFlowNode.h"
#include "GameFramework/Actor.h"
#include "TestControlFlow.generated.h"

UCLASS()
class TESTCONTROLFLOW_API ATestControlFlows : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestControlFlows();

#pragma region TestControlFlows

	UFUNCTION(BlueprintCallable, Category = "TestControlFlowsActor")
	void TestQueueStep();

	UFUNCTION(BlueprintCallable, Category = "TestControlFlowsActor")
	void TestQueueWait();

private:

	void QueueStepFunction();
	
	void QueueWaitFunction(FControlFlowNodeRef SubFlow);

	void Construct();

	void Destruct();

#pragma endregion TestControlFlows
};
