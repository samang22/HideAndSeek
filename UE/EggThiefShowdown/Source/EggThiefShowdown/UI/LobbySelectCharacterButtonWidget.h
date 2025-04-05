// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Misc/Utils.h"
#include "LobbySelectCharacterButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonEvent, const FString&, UserName);

UCLASS()
class EGGTHIEFSHOWDOWN_API ULobbySelectCharacterButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	friend class ALobbyCharacter;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* SelectionButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatusTextBlock;

protected:
	LOBBY_SELECT_CHARACTER_BUTTON_STATUS eButtonStatus = LOBBY_SELECT_CHARACTER_BUTTON_STATUS::DESELECTED;
public:
	LOBBY_SELECT_CHARACTER_BUTTON_STATUS GetButtonStatus() const { return eButtonStatus; }

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

public:
	// 버튼 누르는 것이 성공했을 때 호출
	void ButtonClickSuccess();
	void UpdateWidgetColor();

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnButtonEvent OnButtonEvent;  // 버튼 클릭 이벤트를 전달하는 Delegate
};
