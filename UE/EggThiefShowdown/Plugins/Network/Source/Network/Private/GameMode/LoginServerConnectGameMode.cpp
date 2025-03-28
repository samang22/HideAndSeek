// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LoginServerConnectGameMode.h"
#include "Subsystem/HaServerSubsystem.h"

void ALoginServerConnectGameMode::BeginPlay()
{
	Super::BeginPlay();

	UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();
	Subsystem->ConnectToServer(Host, Port);
}
