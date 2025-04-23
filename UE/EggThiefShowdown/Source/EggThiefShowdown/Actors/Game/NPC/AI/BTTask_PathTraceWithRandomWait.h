// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Game/NPC/AI/BTTask_Base.h"
#include "AIController.h"
#include "BTTask_PathTraceWithRandomWait.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API UBTTask_PathTraceWithRandomWait : public UBTTask_Base
{
	GENERATED_BODY()
public:
	UBTTask_PathTraceWithRandomWait();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void OnResult(EPathFollowingResult::Type MovementResult);

	UPROPERTY()
	class UAIAsyncTaskBlueprintProxy* Proxy;

	class UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;
	class UBlackboardComponent* BlackboardComponent = nullptr;
	class USplineComponent* SplineComponent = nullptr;
	int32 CurrentPatrolIndex = 0;
	int32 SplinePoints = 0;

public:
	float WaitTime = 0.0f;
	float ElapsedTime = 0.0f;
	bool bWaiting = false;
};
