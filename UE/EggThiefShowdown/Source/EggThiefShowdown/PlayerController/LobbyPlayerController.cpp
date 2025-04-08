// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "../Actors/Lobby/LobbyCharacter.h"
#include "../GameMode/LobbyMapGameMode.h"

void ALobbyPlayerController::Server_SelectLobbyCharacter_Implementation(ALobbyCharacter* LobbyCharacter, const FString& InUserName)
{
    if (LobbyCharacter)
    {
        LobbyCharacter->ServerSelectActor(InUserName);

        // 요청한 UserName과 LobbyCharacter의 UserName의 이름이 같다면, 이름이 바뀌었다는 것
        if (LobbyCharacter->GetUserName() == InUserName)
        {
            SetSelectedLobbyCharacter(LobbyCharacter);
        }
        else
        {
            SetSelectedLobbyCharacter(nullptr);
        }
    }

    ALobbyMapGameMode* GameMode = Cast<ALobbyMapGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->CheckAndServerTravel();
    }
}
