// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Misc/Utils.h"
#include "LobbyYoshiActor.generated.h"
class USkeletalMeshComponent;

UCLASS()
class EGGTHIEFSHOWDOWN_API ALobbyYoshiActor : public AActor
{
	GENERATED_BODY()

	virtual void PostInitProperties() override;
public:	
	// Sets default values for this actor's properties
	ALobbyYoshiActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

public:
	void PlayMontage(LOBBY_CHARACTER_ENUM _InEnum, bool bIsLoop = false);
	bool IsMontage(LOBBY_CHARACTER_ENUM _InEnum);
	bool IsPlayingMontage(LOBBY_CHARACTER_ENUM _InEnum);
};
