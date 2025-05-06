// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "CustomSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API ACustomSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:
	ACustomSpectatorPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
