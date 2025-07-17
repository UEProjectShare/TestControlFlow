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

	UFUNCTION(BlueprintCallable, Category = "TestControlFlowsActor")
	void TestQueueControlFlow();

	UFUNCTION(BlueprintCallable, Category = "TestControlFlowsActor")
	void TestQueueControlFlowBranch();

	UFUNCTION(BlueprintCallable, Category = "TestControlFlowsActor")
	void TestQueueConcurrentFlows();

private:

	void QueueStepFunction();
	
	void QueueWaitFunction(FControlFlowNodeRef SubFlow);
	
	void QueueControlFlow(TSharedRef<FControlFlow> FlowRef, int32 Param1, FString Param2);
	
	void QueueControlFlowBranch1(int32 Param1, FString Param2);
	
	void QueueControlFlowBranch2(int32 Param1, FString Param2);
	
	void QueueConcurrentFlows1(int32 Param1, FString Param2);
	
	void QueueConcurrentFlows2(int32 Param1, FString Param2);

	void Construct();

	void Destruct();

#pragma endregion TestControlFlows
};
