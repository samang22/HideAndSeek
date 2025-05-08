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
#include "GameFramework/SpectatorPawn.h"
#include "Actors/Game/Character/CustomSpectatorPawn.h"
#include "Actors/Game/NPC/RealYoshiAIController.h"
#include "GameState/GameMapGameState.h"

#include "Actors/Game/TriggerBox/EggCountTriggerBox.h"

AGameMapGameMode::AGameMapGameMode()
{
    PlayerStateClass = AGameMapPlayerState::StaticClass();
	SpectatorClass = ACustomSpectatorPawn::StaticClass(); 
	GameStateClass = AGameMapGameState::StaticClass();
}

void AGameMapGameMode::SetPlayerData(AController* NewPlayer)
{
	UE_LOG(LogGameMode, Warning, TEXT("AGameMapGameMode::SetPlayerData"));

}

void AGameMapGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartCountdown();
}

void AGameMapGameMode::HandlePlayerDeath(AController* PlayerController)
{
	if (!PlayerController)
	{
		UE_LOG(LogGameMode, Warning, TEXT("PlayerController is null in HandlePlayerDeath"));
		return;
	}

	// UnPossess를 호출하기 전에 현재 Pawn의 위치를 저장
	const APawn* CurrentPawn = PlayerController->GetPawn();
	const FVector PawnLocation = CurrentPawn ? CurrentPawn->GetActorLocation() : FVector::ZeroVector;

	// 현재 플레이어가 소유한 Pawn을 UnPossess
	PlayerController->UnPossess();

	// 관전자 Pawn 생성
	ACustomSpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ACustomSpectatorPawn>(SpectatorClass, PawnLocation, FRotator::ZeroRotator);
	if (SpectatorPawn)
	{
		// 플레이어 상태를 관전자로 변경
		PlayerController->ChangeState(NAME_Spectating);
		// 관전자 Pawn을 플레이어 컨트롤러가 소유하도록 설정
		PlayerController->Possess(SpectatorPawn);

		UE_LOG(LogGameMode, Log, TEXT("Player has been switched to spectator mode."));
	}
	else
	{
		UE_LOG(LogGameMode, Error, TEXT("Failed to spawn SpectatorPawn."));
	}
}

void AGameMapGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGameMapGameMode::StartCountdown()
{
	CountdownTime = 10;
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AGameMapGameMode::CountdownTick, 1.f, true);

	// Player들의 움직임 제한
	EnablePlayersMove(false);
	// AIController들의 움직임 제한
	EnableAIControllers(false);
}

void AGameMapGameMode::CountdownTick()
{
	CountdownTime--;
	UE_LOG(LogGameMode, Warning, TEXT("Countdown: %d"), CountdownTime);


	// PlayerState 또는 PlayerController에 남은 시간 전달

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(*It))
		{
			PC->Client_UpdateCountdown(CountdownTime); // RPC로 클라이언트 UI 갱신
		}
	}

	if (CountdownTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		StartGame(); // 본격 게임 시작
	}
}

void AGameMapGameMode::StartGame()
{
	EnablePlayersMove(true);
	EnableAIControllers(true);
}

void AGameMapGameMode::UpdateEggCount()
{
	TArray<AActor*> FoundEggTriggerBox;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEggCountTriggerBox::StaticClass(), FoundEggTriggerBox);

	int32 EggCount = 0;
	for (AActor* Actor : FoundEggTriggerBox)
	{
		if (AEggCountTriggerBox* TriggerBox = Cast<AEggCountTriggerBox>(Actor))
		{
			EggCount += TriggerBox->GetEggCount();
		}
	}

	// GameMapGameState에 EggCount 설정
	if (AGameMapGameState* GameMapGameState = GetGameState<AGameMapGameState>())
	{
		float EggGauge = GameMapGameState->GetEggGauge();
		EggGauge += EggCount * EGG_GAUGE_COEFFICIENT; // EggGauge를 EggCount에 비례하여 증가    
		GameMapGameState->SetEggGauge(EggGauge);
	}
}

void AGameMapGameMode::EnablePlayersMove(bool bFlag)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(*It))
		{
			if (AGamePlayer* GamePlayer = Cast<AGamePlayer>(PC->GetPawn()))
			{
				GamePlayer->SetCanMove(bFlag);
			}
		}
	}
}

void AGameMapGameMode::EnableAIControllers(bool bFlag)
{
	TArray<AActor*> FoundAIControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARealYoshiAIController::StaticClass(), FoundAIControllers);

	for (AActor* Actor : FoundAIControllers)
	{
		if (ARealYoshiAIController* AIController = Cast<ARealYoshiAIController>(Actor))
		{
			AIController->SetAIEnabled(bFlag);
		}
	}
}
