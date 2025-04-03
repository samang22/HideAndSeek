// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySelectBtnWidget.h"
#include "Subsystem/HaServerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void ULobbySelectBtnWidget::NativeConstruct()
{
    if (SelectionButton)
    {
        SelectionButton->OnClicked.AddDynamic(this, &ULobbySelectBtnWidget::OnButtonClicked);
    }
}

void ULobbySelectBtnWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void ULobbySelectBtnWidget::OnButtonClicked()
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

void ULobbySelectBtnWidget::SetButtonColor(const FLinearColor& NewColor)
{
    FButtonStyle ButtonStyle = SelectionButton->WidgetStyle;
    ButtonStyle.Normal.TintColor = FSlateColor(NewColor);
    ButtonStyle.Hovered.TintColor = FSlateColor(NewColor);
    ButtonStyle.Pressed.TintColor = FSlateColor(NewColor);
    SelectionButton->SetStyle(ButtonStyle);
}
