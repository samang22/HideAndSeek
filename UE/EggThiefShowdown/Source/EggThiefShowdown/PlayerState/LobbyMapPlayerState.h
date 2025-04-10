// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "../Misc/Utils.h"
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
    uint8 eLobbyCharacterKind = (uint8)LOBBY_CHARACTER_KIND::END;
public:
    void SetLobbyCharacterKind(uint8 eKind) { eLobbyCharacterKind = eKind; }
    uint8 GetLobbyCharacterKind() const { return eLobbyCharacterKind; }

//protected:
//    FDataTableRowHandle DataTableRowHandle;
//public:
//    void SetDataTableRowHandle(FDataTableRowHandle Handle) { DataTableRowHandle = Handle; }
//    FDataTableRowHandle GetDataTableRowHandle() const { return DataTableRowHandle; }
//
//protected:
//    FVector SpawnLocation = FVector::ZeroVector;
//    FRotator SpawnRotation = FRotator::ZeroRotator;
//public:
//    void SetSpawnLocation(FVector InVector) { SpawnLocation = InVector; }
//    FVector GetSpawnLocation() { return SpawnLocation; }
//    void SetSpawnRotation(FRotator InRotator) { SpawnRotation = InRotator; }
//    FRotator GetSpawnRotation() { return SpawnRotation; }


public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void CopyProperties(APlayerState* NewPlayerState) override;
};
