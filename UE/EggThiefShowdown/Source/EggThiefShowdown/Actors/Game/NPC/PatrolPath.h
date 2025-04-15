// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class USplineComponent;

UCLASS()
class EGGTHIEFSHOWDOWN_API APatrolPath : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APatrolPath();

	USplineComponent* GetPath() { return SplineComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	USplineComponent* SplineComponent;

};
