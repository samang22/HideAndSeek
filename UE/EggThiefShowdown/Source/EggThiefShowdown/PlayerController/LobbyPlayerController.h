// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class ALobbyCharacter;

UCLASS()
class EGGTHIEFSHOWDOWN_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ALobbyPlayerController();
	UFUNCTION(Server, Reliable)
	void Server_SelectLobbyCharacter(ALobbyCharacter* LobbyCharacter, const FString& InUserName);

protected:
	TObjectPtr<AActor> SelectedLobbyCharacter = nullptr;
public:
	void SetSelectedLobbyCharacter(AActor* InCharacter);
	AActor* GetSelectedLobbyCharacter() const { return SelectedLobbyCharacter.Get(); }


public:
	virtual void BeginPlay() override;
};
