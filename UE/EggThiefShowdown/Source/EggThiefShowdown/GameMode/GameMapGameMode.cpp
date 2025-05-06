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

AGameMapGameMode::AGameMapGameMode()
{
    PlayerStateClass = AGameMapPlayerState::StaticClass();

	SpectatorClass = ACustomSpectatorPawn::StaticClass(); // 커스텀 SpectatorPawn도 가능
}

void AGameMapGameMode::SetPlayerData(AController* NewPlayer)
{
	UE_LOG(LogGameMode, Warning, TEXT("AGameMapGameMode::SetPlayerData"));

}

void AGameMapGameMode::BeginPlay()
{
	Super::BeginPlay();
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