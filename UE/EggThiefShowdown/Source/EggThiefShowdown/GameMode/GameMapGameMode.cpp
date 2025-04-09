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

AGameMapGameMode::AGameMapGameMode()
{
    PlayerStateClass = AGameMapPlayerState::StaticClass();
}

void AGameMapGameMode::PostSeamlessTravel()
{
    Super::PostSeamlessTravel();

    UE_LOG(LogTemp, Warning, TEXT("AGameMapGameMode::PostSeamlessTravel"));

    // PlayerState의 정보를 토대로 초기화
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        APlayerController* PC = Iterator->Get();
        AGameMapPlayerState* NewPlayerState = PC->GetPlayerState<AGameMapPlayerState>();
        
        if (NewPlayerState)
        {
            static UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_GamePlayer.DT_GamePlayer"));

            switch (static_cast<LOBBY_CHARACTER_KIND>(NewPlayerState->GetCharacterKind()))
            {
            case LOBBY_CHARACTER_KIND::MARIO:
            {
                UE_LOG(LogTemp, Warning, TEXT("MARIO"));

                TArray<AActor*> SpawnPoints;
                UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMarioSpawnPoint::StaticClass(), SpawnPoints);

                if (SpawnPoints.Num() > 0)
                {
                    // 2. 랜덤 스폰 포인트 선택
                    FVector SpawnLocation = {};
                    FRotator SpawnRotation = {};

                    while (true)
                    {
                        int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
                        AMarioSpawnPoint* MarioSP = Cast<AMarioSpawnPoint>(SpawnPoints[RandomIndex]);
                        if (MarioSP->GetIsSpawned()) continue;

                        MarioSP->SetIsSpawned(true);
                        SpawnLocation = MarioSP->GetActorLocation();
                        SpawnRotation = MarioSP->GetActorRotation();
                        break;
                    }

                    // 3. Pawn 스폰 및 소유권 설정
                    FActorSpawnParameters SpawnParams;
                    SpawnParams.Owner = PC;
                    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(AGamePlayer::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

                    if (NewPawn)
                    {
                        PC->Possess(NewPawn);
                    }

                    AGamePlayer* GamePlayer = Cast<AGamePlayer>(NewPawn);
                    if (GamePlayer)
                    {
                        if (DataTable)
                        {
                            FDataTableRowHandle DataTableRowHandle;
                            DataTableRowHandle.DataTable = DataTable;
                            DataTableRowHandle.RowName = FName(TEXT("Mario"));
                            GamePlayer->SetData(DataTableRowHandle);
                        }
 
                    }
                }
            }

                break;
            case LOBBY_CHARACTER_KIND::YOSHI:
            {
                UE_LOG(LogTemp, Warning, TEXT("YOSHI"));

                TArray<AActor*> SpawnPoints;
                UGameplayStatics::GetAllActorsOfClass(GetWorld(), AYoshiSpawnPoint::StaticClass(), SpawnPoints);

                if (SpawnPoints.Num() > 0)
                {
                    // 2. 랜덤 스폰 포인트 선택
                    FVector SpawnLocation = {};
                    FRotator SpawnRotation = {};

                    while (true)
                    {
                        int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
                        AYoshiSpawnPoint* YoshiSP = Cast<AYoshiSpawnPoint>(SpawnPoints[RandomIndex]);
                        if (YoshiSP->GetIsSpawned()) continue;

                        YoshiSP->SetIsSpawned(true);
                        SpawnLocation = YoshiSP->GetActorLocation();
                        SpawnRotation = YoshiSP->GetActorRotation();
                        break;
                    }

                    // 3. Pawn 스폰 및 소유권 설정
                    FActorSpawnParameters SpawnParams;
                    SpawnParams.Owner = PC;
                    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(AGamePlayer::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

                    if (NewPawn)
                    {
                        PC->Possess(NewPawn);
                    }

                    AGamePlayer* GamePlayer = Cast<AGamePlayer>(NewPawn);
                    if (GamePlayer)
                    {

                        if (DataTable)
                        {
                            FDataTableRowHandle DataTableRowHandle;
                            DataTableRowHandle.DataTable = DataTable;
                            DataTableRowHandle.RowName = FName(TEXT("Yoshi"));
                            GamePlayer->SetData(DataTableRowHandle);
                        }

                    }
                }
            }
                break;
            default:
                break;
            }
        }
    }

}
