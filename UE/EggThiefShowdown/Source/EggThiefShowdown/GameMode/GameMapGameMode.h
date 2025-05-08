// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API AGameMapGameMode : public AGameMode
{
	GENERATED_BODY()
	

public:
	AGameMapGameMode();
	//virtual void PostSeamlessTravel() override;
	void SetPlayerData(AController* NewPlayer);

	virtual void BeginPlay() override;
	virtual void HandlePlayerDeath(AController* PlayerController);

protected:
	virtual void Tick(float DeltaSeconds) override;


protected:
	FTimerHandle CountdownTimerHandle;
	int32 CountdownTime = 10;

protected:
	void StartCountdown();
	void CountdownTick();
	void StartGame();

public:
	void UpdateEggCountAndCheckEnd(float _fDeltaTime);
protected:
	void EnablePlayersMove(bool bFlag);
	void EnableAIControllers(bool bFlag);
};
