// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Game/NPC/AI/BTTask_Base.h"
#include "AIController.h"
#include "BTTask_PathTrace.generated.h"

UCLASS()
class EGGTHIEFSHOWDOWN_API UBTTask_PathTrace : public UBTTask_Base
{
	GENERATED_BODY()
public:
	UBTTask_PathTrace();

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
};
