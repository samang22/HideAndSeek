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
	RemainingTime = GAME_TIME_LIMIT;
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
	UpdateEggCountAndCheckEnd(DeltaSeconds);
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

	GetWorld()->GetTimerManager().SetTimer(
		TimeLimitTimerHandle,
		this,
		&AGameMapGameMode::UpdateTimeLimitWidget,
		1.0f, // 매 초마다 호출
		true  // 반복 실행
	);
	
	UpdateTimeLimitWidget();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(*It))
		{
			if (AGamePlayer* GamePlayer = Cast<AGamePlayer>(PC->GetPawn()))
			{
				PC->Client_ExposeTimeLimitWidget(true);
			}
		}
	}
}

void AGameMapGameMode::ServerTravelToLobbyMap()
{
	// 서버 여행을 위한 URL 설정
	FString TravelURL = TEXT("/Game/Level/LobbyMap?game=/Game/GameMode/BPGM_LobbyMap.BPGM_LobbyMap_C");
	GetWorld()->ServerTravel(TravelURL);
}

void AGameMapGameMode::UpdateEggCountAndCheckEnd(float _fDeltaTime)
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

	if (0 == EggCount) return;

	float EggGauge = 0.f;
	// GameMapGameState에 EggCount 설정
	if (AGameMapGameState* GameMapGameState = GetGameState<AGameMapGameState>())
	{
		EggGauge = GameMapGameState->GetEggGauge();
		EggGauge += EggCount * EGG_GAUGE_COEFFICIENT * _fDeltaTime; // EggGauge를 EggCount에 비례하여 증가    
		GameMapGameState->SetEggGauge(EggGauge);
	}

	if (EggGauge >= 1.f)
	{
		// EggGauge가 1.0 이상이면 게임 종료
		EnablePlayersMove(false);
		EnableAIControllers(false);

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(*It))
			{
				if (AGamePlayer* GamePlayer = Cast<AGamePlayer>(PC->GetPawn()))
				{
					PC->Client_ExposeGameResultWidget(true);


					const LOBBY_CHARACTER_KIND eKind = GamePlayer->GetCharacterKind();
					switch (eKind)
					{
						case LOBBY_CHARACTER_KIND::MARIO:
							PC->Client_UpdateGameEnd(false);
							break;
						case LOBBY_CHARACTER_KIND::YOSHI:
							PC->Client_UpdateGameEnd(true);
							break;
						default:
							UE_LOG(LogTemp, Error, TEXT("Unknown character kind"));
						break;
					}
				}
			}
		}

		// 
		GetWorld()->GetTimerManager().SetTimer(
			ServerTravelToLobbyMapTimerHandle,
			this,
			&AGameMapGameMode::ServerTravelToLobbyMap,
			5.0f, 
			false 
		);
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

void AGameMapGameMode::UpdateTimeLimitWidget()
{
	// 남은 시간 감소
	RemainingTime--;

	int minutes = RemainingTime / 60;
	int seconds = RemainingTime % 60;

	int minTens = minutes / 10;
	int minOnes = minutes % 10;
	int secTens = seconds / 10;
	int secOnes = seconds % 10;

	// 위젯 갱신 로직
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(*It))
		{
			PC->Client_UpdateTimeLimit(minTens, minOnes, secTens, secOnes);
		}
	}


	// 시간이 0이 되면 타이머 중지
	if (RemainingTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeLimitTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Time is up!"));

		EnablePlayersMove(false);
		EnableAIControllers(false);

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(*It))
			{
				if (AGamePlayer* GamePlayer = Cast<AGamePlayer>(PC->GetPawn()))
				{
					PC->Client_ExposeGameResultWidget(true);

					const LOBBY_CHARACTER_KIND eKind = GamePlayer->GetCharacterKind();
					switch (eKind)
					{
					case LOBBY_CHARACTER_KIND::MARIO:
						PC->Client_UpdateGameEnd(true);
						break;
					case LOBBY_CHARACTER_KIND::YOSHI:
						PC->Client_UpdateGameEnd(false);
						break;
					default:
						UE_LOG(LogTemp, Error, TEXT("Unknown character kind"));
						break;
					}
				}
			}
		}

		GetWorld()->GetTimerManager().SetTimer(
			ServerTravelToLobbyMapTimerHandle,
			this,
			&AGameMapGameMode::ServerTravelToLobbyMap,
			5.0f, // 3초 후에 서버 여행
			false // 단발성 실행
		);
	}
}
