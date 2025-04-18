// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Game/NPC/AI/BTTask_Base.h"
#include "BTTask_CareEgg.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API UBTTask_CareEgg : public UBTTask_Base
{
	GENERATED_BODY()
public:
	UBTTask_CareEgg();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	class UBlackboardComponent* BlackboardComponent = nullptr;

};
