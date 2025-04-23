// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Game/NPC/AI/BTTask_Base.h"
#include "AIController.h"
#include "BTTask_MoveToEgg.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAIAsyncTaskBlueprintProxy;

UCLASS()
class EGGTHIEFSHOWDOWN_API UBTTask_MoveToEgg : public UBTTask_Base
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToEgg();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UFUNCTION()
	void OnResult(EPathFollowingResult::Type MovementResult);
protected:
	UPROPERTY()
	UAIAsyncTaskBlueprintProxy* Proxy;

	UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;
	UBlackboardComponent* BlackboardComponent = nullptr;
};
