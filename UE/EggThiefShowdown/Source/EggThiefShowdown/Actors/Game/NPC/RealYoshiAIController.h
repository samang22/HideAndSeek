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
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetPatrolPath(TObjectPtr<USplineComponent> NewPatrolPath, APawn* OwningPawn);
protected:
	void FindEggByPerception();

};
