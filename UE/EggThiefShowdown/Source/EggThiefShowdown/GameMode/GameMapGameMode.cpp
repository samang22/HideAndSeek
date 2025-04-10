// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMapGameMode.h"
#include "Subsystem/HaServerSubsystem.h"
#include "../Misc/Utils.h"
#include "../Actors/Game/Character/GamePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/Game/SpawnPoint/MarioSpawnPoint.h"
#include "../Actors/Game/SpawnPoint/YoshiSpawnPoint.h"
#include "../PlayerController/LobbyPlayerController.h"
#include "../PlayerState/GameMapPlayerState.h"
#include "../PlayerState/LobbyMapPlayerState.h"

AGameMapGameMode::AGameMapGameMode()
{
    PlayerStateClass = AGameMapPlayerState::StaticClass();
}

void AGameMapGameMode::SetPlayerData(AController* NewPlayer)
{
	UE_LOG(LogGameMode, Warning, TEXT("AGameMapGameMode::SetPlayerData"));


	//if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	//{
	//	UE_LOG(LogGameMode, Warning, TEXT("NewPlayer == nullptr || NewPlayer->IsPendingKillPending()"));
 //
	//	return;
	//}
	//if (NewPlayer)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::PC"));

	//	if (ALobbyPlayerController* LPC = Cast< ALobbyPlayerController>(NewPlayer))
	//	{
	//		if (ALobbyMapPlayerState* NewPlayerState = LPC->GetPlayerState<ALobbyMapPlayerState>())
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::ALobbyMapPlayerState"));

	//			static UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_GamePlayer.DT_GamePlayer"));

	//			switch (static_cast<LOBBY_CHARACTER_KIND>(NewPlayerState->GetLobbyCharacterKind()))
	//			{
	//			case LOBBY_CHARACTER_KIND::MARIO:
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("MARIO"));

	//				TArray<AActor*> SpawnPoints;
	//				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMarioSpawnPoint::StaticClass(), SpawnPoints);

	//				if (SpawnPoints.Num() > 0)
	//				{
	//					// 2. 랜덤 스폰 포인트 선택
	//					FVector SpawnLocation = {};
	//					FRotator SpawnRotation = {};

	//					while (true)
	//					{
	//						int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
	//						AMarioSpawnPoint* MarioSP = Cast<AMarioSpawnPoint>(SpawnPoints[RandomIndex]);
	//						if (MarioSP->GetIsSpawned()) continue;

	//						MarioSP->SetIsSpawned(true);
	//						break;
	//					}

	//					if (DataTable)
	//					{
	//						FDataTableRowHandle tempDataTableRowHandle;
	//						tempDataTableRowHandle.DataTable = DataTable;
	//						tempDataTableRowHandle.RowName = FName(TEXT("Mario"));
	//						NewPlayerState->SetDataTableRowHandle(tempDataTableRowHandle);
	//					}
	//				}
	//			}

	//			break;
	//			case LOBBY_CHARACTER_KIND::YOSHI:
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("YOSHI"));

	//				TArray<AActor*> SpawnPoints;
	//				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AYoshiSpawnPoint::StaticClass(), SpawnPoints);

	//				if (SpawnPoints.Num() > 0)
	//				{
	//					// 2. 랜덤 스폰 포인트 선택
	//					FVector SpawnLocation = {};
	//					FRotator SpawnRotation = {};

	//					while (true)
	//					{
	//						int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
	//						AYoshiSpawnPoint* YoshiSP = Cast<AYoshiSpawnPoint>(SpawnPoints[RandomIndex]);
	//						if (YoshiSP->GetIsSpawned()) continue;

	//						YoshiSP->SetIsSpawned(true);
	//						StartSpot = YoshiSP;
	//						break;
	//					}


	//					if (DataTable)
	//					{
	//						FDataTableRowHandle tempDataTableRowHandle;
	//						tempDataTableRowHandle.DataTable = DataTable;
	//						tempDataTableRowHandle.RowName = FName(TEXT("Yoshi"));
	//						NewPlayerState->SetDataTableRowHandle(tempDataTableRowHandle);
	//					}
	//				}
	//			}
	//			break;
	//			default:
	//				UE_LOG(LogTemp, Warning, TEXT("no MARIO no YOSHI"));
	//				return;
	//				break;
	//			}
	//		}
	//	}
	//}
}

void AGameMapGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 현재 월드에서 모든 플레이어 컨트롤러를 순회
	//for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	//{
	//	APlayerController* PlayerController = Iterator->Get();
	//	if (PlayerController && PlayerController->GetPawn() == nullptr)
	//	{
	//		SetPlayerData(PlayerController);
	//	}
	//}
}

//void AGameMapGameMode::PostSeamlessTravel()
//{
//    Super::PostSeamlessTravel();
//
//    UE_LOG(LogTemp, Warning, TEXT("AGameMapGameMode::PostSeamlessTravel"));
//
//}
