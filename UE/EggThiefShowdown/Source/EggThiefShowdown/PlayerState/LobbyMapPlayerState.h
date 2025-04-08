// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyMapPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API ALobbyMapPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(Replicated)
    uint8 eLobbyCharacterKind = 0;
public:
    void SetLobbyCharacterKind(uint8 eKind) { eLobbyCharacterKind = eKind; }
    uint8 GetLobbyCharacterKind() const { return eLobbyCharacterKind; }

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void CopyProperties(APlayerState* NewPlayerState) override;
};
