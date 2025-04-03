// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Misc/Utils.h"
#include "GameFramework/Actor.h"
#include "LobbyYoshi.generated.h"

class USkeletalMeshComponent;
class ULobbyCharacterStatusComponent;
struct FLobbyCharacterTableRow;

UCLASS()
class EGGTHIEFSHOWDOWN_API ALobbyYoshi : public AActor
{
	GENERATED_BODY()

	virtual void PostInitProperties() override;
public:	
	// Sets default values for this actor's properties
	ALobbyYoshi();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULobbyCharacterStatusComponent> StatusComponent;
public:
	TObjectPtr<ULobbyCharacterStatusComponent> GetStatusComponent() { return StatusComponent; }

protected:
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle DataTableRowHandle;
	FLobbyCharacterTableRow* LobbyCharacterData;


public:
	void PlayMontage(LOBBY_CHARACTER_MONTAGE _InEnum, bool bIsLoop = false);
	bool IsMontage(LOBBY_CHARACTER_MONTAGE _InEnum);
	bool IsPlayingMontage(LOBBY_CHARACTER_MONTAGE _InEnum);
};
