// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "../Actors/Lobby/LobbyCharacter.h"
#include "../GameMode/LobbyMapGameMode.h"
#include "Subsystem/HaServerSubsystem.h"
#include "../PlayerState/LobbyMapPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/Lobby/LobbyCamera.h"

ALobbyPlayerController::ALobbyPlayerController()
{

}

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

void ALobbyPlayerController::SetSelectedLobbyCharacter(AActor* InCharacter)
{
    SelectedLobbyCharacter = InCharacter;
    if (SelectedLobbyCharacter)
    {
        // Local
        UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();
        _ASSERT(Subsystem);
        ALobbyCharacter* LC = Cast<ALobbyCharacter>(SelectedLobbyCharacter);
        LOBBY_CHARACTER_KIND eKind = LC->GetLobbyCharacterKind();
        Subsystem->SetLobbyCharacterKind(static_cast<uint8>(eKind));

        // Save it to next Level (Game Map)
        ALobbyMapPlayerState* LobbyMapPlayerState = Cast<ALobbyMapPlayerState>(PlayerState);
        LobbyMapPlayerState->SetLobbyCharacterKind(static_cast<uint8>(eKind));
    }
}

void ALobbyPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

