// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YoshiSpawnPoint.generated.h"

UCLASS()
class EGGTHIEFSHOWDOWN_API AYoshiSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AYoshiSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	bool bIsSpawned = false;
public:
	void SetIsSpawned(bool bFlag) { bIsSpawned = bFlag; }
	bool GetIsSpawned() const { return bIsSpawned; }
};
