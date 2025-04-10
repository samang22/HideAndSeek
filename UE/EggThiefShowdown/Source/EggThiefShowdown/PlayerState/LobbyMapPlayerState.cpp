// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMapPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameMapPlayerState.h"

void ALobbyMapPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyMapPlayerState, eLobbyCharacterKind);
}

void ALobbyMapPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyMapPlayerState::CopyProperties"));

	Super::CopyProperties(NewPlayerState);

	if (ALobbyMapPlayerState* NewPS = Cast<ALobbyMapPlayerState>(NewPlayerState))
	{
		NewPS->SetLobbyCharacterKind(this->GetLobbyCharacterKind());
	}
}
