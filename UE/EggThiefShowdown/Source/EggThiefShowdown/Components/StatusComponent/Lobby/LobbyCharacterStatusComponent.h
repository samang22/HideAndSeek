// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Misc/Utils.h"
#include "../StatusComponent.h"
#include "LobbyCharacterStatusComponent.generated.h"

UCLASS()
class EGGTHIEFSHOWDOWN_API ULobbyCharacterStatusComponent : public UStatusComponent
{
	GENERATED_BODY()
public:
	ULobbyCharacterStatusComponent();


public:
	void SetOnAnimationStatus(uint8 InBit);
	void SetOffAnimationStatus(uint8 InBit);

public:
	UFUNCTION()
	bool GetAnimStatus(uint8 InBit) const;

protected:
	UPROPERTY()
	uint8 LobbyCharacterAnimationStatus;
	LOBBY_CHARACTER_STATUS eLobbyCharacterStatus = LOBBY_CHARACTER_STATUS::IDLE;
public:
	void SetLobbyCharacterStatus(LOBBY_CHARACTER_STATUS eStatus);
	LOBBY_CHARACTER_STATUS GetLobbyCharacterStatus() const { return eLobbyCharacterStatus; };


protected:
	LOBBY_CHARACTER_KIND eLobbyCharacterKind = LOBBY_CHARACTER_KIND::END;
public:
	void SetLobbyCharacterKind(LOBBY_CHARACTER_KIND eKind) { eLobbyCharacterKind = eKind; }
	LOBBY_CHARACTER_KIND GetLobbyCharacterKind() const { return eLobbyCharacterKind; }
};
