// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NetworkGameMode.generated.h"


UCLASS()
class NETWORK_API ANetworkGameMode : public AGameModeBase, public FNetworkNotify
{
	GENERATED_BODY()
	
public:
	ANetworkGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
