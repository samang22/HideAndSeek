// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameMode.h"
#include "HaNetDriver.h"
#include "Subsystem/HaServerSubsystem.h"

ANetworkGameMode::ANetworkGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANetworkGameMode::BeginPlay()
{
	Super::BeginPlay();

	UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();
	Subsystem->ConnectToServer();
}

void ANetworkGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
