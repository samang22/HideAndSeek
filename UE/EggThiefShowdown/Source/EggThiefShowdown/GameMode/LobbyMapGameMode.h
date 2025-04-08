// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyMapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API ALobbyMapGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALobbyMapGameMode();
	void CheckAndServerTravel();

};
