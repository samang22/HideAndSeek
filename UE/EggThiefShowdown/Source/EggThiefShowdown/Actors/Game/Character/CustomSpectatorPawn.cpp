// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/Character/CustomSpectatorPawn.h"

ACustomSpectatorPawn::ACustomSpectatorPawn(const FObjectInitializer& ObjectInitializer)
{
	bReplicates = true;
	bAlwaysRelevant = true;
}
