// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySelectButtonWidget.h"
#include "Subsystem/HaServerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void ULobbySelectButtonWidget::NativeConstruct()
{
    if (SelectionButton)
    {
        SelectionButton->OnClicked.AddDynamic(this, &ULobbySelectButtonWidget::OnButtonClicked);
    }
}

void ULobbySelectButtonWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void ULobbySelectButtonWidget::OnButtonClicked()
{
    if (LOBBY_SELECT_BUTTON_STATUS::SELECTED == eButtonStatus)
    {
        eButtonStatus = LOBBY_SELECT_BUTTON_STATUS::UNSELECTED;
        switch (eLobbyCharacterEnum)
        {
        case LOBBY_CHARACTER_ENUM::MARIO:
            SetButtonColor(FLinearColor::Red);
            break;
        case LOBBY_CHARACTER_ENUM::YOSHI:
            SetButtonColor(FLinearColor::Green);
            break;
        default:
            break;
        }
        StatusTextBlock->SetText(FText::FromString(TEXT("선택")));
    }
    else
    {
        eButtonStatus = LOBBY_SELECT_BUTTON_STATUS::SELECTED;
        SetButtonColor(FLinearColor::Gray);

        switch (eLobbyCharacterEnum)
        {
        case LOBBY_CHARACTER_ENUM::MARIO:
            StatusTextBlock->SetColorAndOpacity(FLinearColor::Red);
            break;
        case LOBBY_CHARACTER_ENUM::YOSHI:
            StatusTextBlock->SetColorAndOpacity(FLinearColor::Green);
            break;
        default:
            break;
        }

        StatusTextBlock->SetText(FText::FromString(TEXT("선택됨")));
    }
}

void ULobbySelectButtonWidget::SetButtonColor(const FLinearColor& NewColor)
{
    SelectionButton->SetColorAndOpacity(NewColor);
}
