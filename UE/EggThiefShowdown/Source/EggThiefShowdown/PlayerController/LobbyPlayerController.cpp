// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "../Actors/Lobby/LobbyCharacter.h"

void ALobbyPlayerController::Server_SelectLobbyCharacter_Implementation(ALobbyCharacter* LobbyCharacter, const FString& InUserName)
{
    if (LobbyCharacter)
    {
        LobbyCharacter->ServerSelectActor(InUserName);
    }
}
