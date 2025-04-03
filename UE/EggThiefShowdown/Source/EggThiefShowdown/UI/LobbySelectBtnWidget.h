// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Misc/Utils.h"
#include "LobbySelectBtnWidget.generated.h"

/**
 *
 */
UCLASS()
class NETWORK_API ULobbySelectBtnWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* SelectionButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatusTextBlock;

protected:
	LOBBY_SELECT_BUTTON_STATUS eButtonStatus = LOBBY_SELECT_BUTTON_STATUS::UNSELECTED;
public:
	LOBBY_SELECT_BUTTON_STATUS GetButtonStatus() const { return eButtonStatus; }

protected:
	LOBBY_CHARACTER_ENUM eLobbyCharacterEnum = LOBBY_CHARACTER_ENUM::MARIO;
public:
	void SetLobbyCharacterEnum(LOBBY_CHARACTER_ENUM Enum) { eLobbyCharacterEnum = Enum; }


public:
	// 선택된 캐릭터에 대한 상태
	UFUNCTION()
	void OnButtonClicked();

	// 선택된 버튼 색상 설정
	void SetButtonColor(const FLinearColor& NewColor);
};
