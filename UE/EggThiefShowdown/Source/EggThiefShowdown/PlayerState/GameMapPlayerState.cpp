// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMapPlayerState.h"
#include "Net/UnrealNetwork.h"

void AGameMapPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameMapPlayerState, eCharacterKind);
}
