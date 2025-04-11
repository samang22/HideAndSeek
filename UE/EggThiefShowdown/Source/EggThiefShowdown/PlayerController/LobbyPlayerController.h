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
	UInputMappingContext* IMC_Default = nullptr;
	UPROPERTY()
	class UStatusComponent* StatusComponent;

protected:
	virtual void SetupInputComponent() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;

protected:
	void OnMove(const FInputActionValue& InputActionValue);
	void OnLook(const FInputActionValue& InputActionValue);

public:
	void SetInputModeGameOnly();

};
