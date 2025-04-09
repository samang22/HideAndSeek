// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameMap.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API AGameMap : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

};
