﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class ALobbyCharacter;
class UGamePlayerStatusComponent;
class UInputMappingContext;

UCLASS()
class EGGTHIEFSHOWDOWN_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ALobbyPlayerController();
	virtual void BeginPlay() override;

	// -----------------------------------------------------

public:
	UFUNCTION(Server, Reliable)
	void Server_SelectLobbyCharacter(ALobbyCharacter* LobbyCharacter, const FString& InUserName);

protected:
	TObjectPtr<AActor> SelectedLobbyCharacter = nullptr;
public:
	void SetSelectedLobbyCharacter(AActor* InCharacter);
	AActor* GetSelectedLobbyCharacter() const { return SelectedLobbyCharacter.Get(); }


// ------------------------------------------------------------------------
protected:
	UPROPERTY()
	UInputMappingContext* IMC_Default = nullptr;
	UPROPERTY()
	UGamePlayerStatusComponent* StatusComponent;

protected:
	virtual void SetupInputComponent() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;

protected:
	void OnMove(const FInputActionValue& InputActionValue);
	UFUNCTION(Server, Reliable)
	void Server_OnMove(const FInputActionValue& InputActionValue);
	void OnMoveOff(const FInputActionValue& InputActionValue);
	void OnLook(const FInputActionValue& InputActionValue);
	void OnRun(const FInputActionValue& InputActionValue);
	void OnRunOff(const FInputActionValue& InputActionValue);
	void OnAttack(const FInputActionValue& InputActionValue);
	void OnDropEgg(const FInputActionValue& InputActionValue);
	void OnRightClick(const FInputActionValue& InputActionValue);
	void OnEnter(const FInputActionValue& InputActionValue);

protected:
	bool bStartChat = false;

public:
	void SetInputModeGameOnly(bool bFlag = true);

public:
	UFUNCTION(Client, Reliable)
	void Client_UpdateCountdown(int32 Countdown);
	UFUNCTION(Client, Reliable)
	void Client_UpdateGameEnd(bool bFlag);
	UFUNCTION(Client, Reliable)
	void Client_UpdateTimeLimit(int32 MinuteTen, int32 MinuteOne, int32 SecondTen, int32 SecondOne);
	UFUNCTION(Client, Reliable)
	void Client_ExposeTimeLimitWidget(bool bFlag);
	UFUNCTION(Client, Reliable)
	void Client_ExposeGameResultWidget(bool bFlag);
};
