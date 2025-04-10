// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMap.h"
#include "../Actors/Game/Character/GamePlayer.h"
#include "../Actors/Game/SpawnPoint/MarioSpawnPoint.h"
#include "../Actors/Game/SpawnPoint/YoshiSpawnPoint.h"
#include "../PlayerState/GameMapPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "../PlayerController/LobbyPlayerController.h"
#include "../PlayerState/LobbyMapPlayerState.h"

void AGameMap::BeginPlay()
{
	Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("AGameMap::BeginPlay"));

    // PlayerState의 정보를 토대로 초기화
	//for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	//{
	//	APlayerController* PC = Iterator->Get();

	//	UE_LOG(LogTemp, Warning, TEXT("AGameMapGameMode::RestartPlayer"));

	//	if (PC)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::PC"));

	//		if (ALobbyPlayerController* LPC = Cast< ALobbyPlayerController>(PC))
	//		{
	//			if (ALobbyMapPlayerState* NewPlayerState = LPC->GetPlayerState<ALobbyMapPlayerState>())
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::ALobbyMapPlayerState"));

	//				static UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_GamePlayer.DT_GamePlayer"));

	//				switch (static_cast<LOBBY_CHARACTER_KIND>(NewPlayerState->GetLobbyCharacterKind()))
	//				{
	//				case LOBBY_CHARACTER_KIND::MARIO:
	//				{
	//					UE_LOG(LogTemp, Warning, TEXT("MARIO"));

	//					TArray<AActor*> SpawnPoints;
	//					UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMarioSpawnPoint::StaticClass(), SpawnPoints);

	//					if (SpawnPoints.Num() > 0)
	//					{
	//						// 2. 랜덤 스폰 포인트 선택
	//						FVector SpawnLocation = {};
	//						FRotator SpawnRotation = {};

	//						while (true)
	//						{
	//							int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
	//							AMarioSpawnPoint* MarioSP = Cast<AMarioSpawnPoint>(SpawnPoints[RandomIndex]);
	//							if (MarioSP->GetIsSpawned()) continue;

	//							MarioSP->SetIsSpawned(true);
	//							LPC->GetPawn()->SetActorLocationAndRotation(MarioSP->GetActorLocation(), MarioSP->GetActorRotation().Quaternion());
	//							break;
	//						}

	//						if (DataTable)
	//						{
	//							FDataTableRowHandle tempDataTableRowHandle;
	//							tempDataTableRowHandle.DataTable = DataTable;
	//							tempDataTableRowHandle.RowName = FName(TEXT("Mario"));
	//							NewPlayerState->SetDataTableRowHandle(tempDataTableRowHandle);
	//						}
	//					}
	//				}

	//				break;
	//				case LOBBY_CHARACTER_KIND::YOSHI:
	//				{
	//					UE_LOG(LogTemp, Warning, TEXT("YOSHI"));

	//					TArray<AActor*> SpawnPoints;
	//					UGameplayStatics::GetAllActorsOfClass(GetWorld(), AYoshiSpawnPoint::StaticClass(), SpawnPoints);

	//					if (SpawnPoints.Num() > 0)
	//					{
	//						// 2. 랜덤 스폰 포인트 선택
	//						FVector SpawnLocation = {};
	//						FRotator SpawnRotation = {};

	//						while (true)
	//						{
	//							int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
	//							AYoshiSpawnPoint* YoshiSP = Cast<AYoshiSpawnPoint>(SpawnPoints[RandomIndex]);
	//							if (YoshiSP->GetIsSpawned()) continue;

	//							YoshiSP->SetIsSpawned(true);
	//							LPC->GetPawn()->SetActorLocationAndRotation(YoshiSP->GetActorLocation(), YoshiSP->GetActorRotation().Quaternion());
	//							break;
	//						}


	//						if (DataTable)
	//						{
	//							FDataTableRowHandle tempDataTableRowHandle;
	//							tempDataTableRowHandle.DataTable = DataTable;
	//							tempDataTableRowHandle.RowName = FName(TEXT("Yoshi"));
	//							NewPlayerState->SetDataTableRowHandle(tempDataTableRowHandle);
	//						}
	//					}
	//				}
	//				break;
	//				default:
	//					UE_LOG(LogTemp, Warning, TEXT("no MARIO no YOSHI"));
	//					return;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
}
