// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Base.generated.h"

class AAIController;

UCLASS()
class EGGTHIEFSHOWDOWN_API UBTTask_Base : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	virtual void SetOwner(AActor* InActorOwner) override;

protected:
	/** Cached AIController owner of BehaviorTreeComponent. */
	UPROPERTY(Transient)
	TObjectPtr<AAIController> AIOwner;

	/** Cached actor owner of BehaviorTreeComponent. */
	UPROPERTY(Transient)
	TObjectPtr<AActor> ActorOwner;
};
