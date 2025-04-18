// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "RealYoshiAIController.generated.h"

class USplineComponent;
class UStatusComponent;
class APawn;

UCLASS()
class EGGTHIEFSHOWDOWN_API ARealYoshiAIController : public ABaseAIController
{
	GENERATED_BODY()

public:
	ARealYoshiAIController();
public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void SetPatrolPath(TObjectPtr<USplineComponent> NewPatrolPath, APawn* OwningPawn);
protected:

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
};
