// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMapGameMode.h"
#include "../PlayerController/LobbyPlayerController.h"
#include "../Actors/Lobby/LobbyCharacter.h"
#include "../Components/StatusComponent/Lobby/LobbyCharacterStatusComponent.h"
#include "../PlayerState/LobbyMapPlayerState.h"

ALobbyMapGameMode::ALobbyMapGameMode()
{
	PlayerStateClass = ALobbyMapPlayerState::StaticClass();
}

void ALobbyMapGameMode::CheckAndServerTravel()
{
	bool bTravel = true;
	bool bIsMario = false;
	uint16 iYoshiNum = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC) { continue; }

		ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC);
		if (!LobbyPC) { continue; }

		if (LobbyPC->GetSelectedLobbyCharacter())
		{
			ALobbyCharacter* LobbyCharacter = Cast<ALobbyCharacter>(LobbyPC->GetSelectedLobbyCharacter());
			ULobbyCharacterStatusComponent* LCSC = LobbyCharacter->GetStatusComponent();
			if (!LCSC) return;
			if (LOBBY_CHARACTER_KIND::MARIO == LCSC->GetLobbyCharacterKind())
			{ 
				bIsMario = true;
			}
			else
			{
				++iYoshiNum;
			}
		}
		else
		{
			bTravel = false;
			break;
		}
	}


	if (bTravel && bIsMario)
	{
		if (iYoshiNum > 0)
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
				{
					FString TravelURL = TEXT("/Game/Level/GameMap?game=/Game/GameMode/BPGM_GameMap.BPGM_GameMap_C");
					GetWorld()->ServerTravel(TravelURL, true); // Seamless Travel
				});
		}
	}


}
