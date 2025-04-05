// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySelectCharacterButtonWidget.h"
#include "Subsystem/HaServerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/HaServerSubsystem.h"

void ULobbySelectCharacterButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SelectionButton)
    {
        SelectionButton->OnClicked.AddDynamic(this, &ULobbySelectCharacterButtonWidget::OnButtonClicked);
    }
    if (StatusTextBlock)
    {
        StatusTextBlock->SetText(FText::FromString(TEXT("선택")));
    }


}

void ULobbySelectCharacterButtonWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);




}

void ULobbySelectCharacterButtonWidget::OnButtonClicked()
{
    UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();
    _ASSERT(Subsystem);
    OnButtonEvent.Broadcast(Subsystem->GetLoginUserName());
}

void ULobbySelectCharacterButtonWidget::SetButtonColor(const FLinearColor& NewColor)
{
    // 버튼 색상 설정 (예: 버튼이 클릭된 상태에 따라 색상 변경)
    if (SelectionButton)
    {
        // 실제 버튼의 스타일을 업데이트할 수 있는 방식으로 처리
        SelectionButton->WidgetStyle.Normal.TintColor = FSlateColor(NewColor);
        SelectionButton->WidgetStyle.Hovered.TintColor = FSlateColor(NewColor);
        SelectionButton->WidgetStyle.Pressed.TintColor = FSlateColor(NewColor);
    }
}

void ULobbySelectCharacterButtonWidget::ButtonClickSuccess()
{
    if (LOBBY_SELECT_CHARACTER_BUTTON_STATUS::SELECTED == eButtonStatus)
    {
        eButtonStatus = LOBBY_SELECT_CHARACTER_BUTTON_STATUS::DESELECTED;
        StatusTextBlock->SetText(FText::FromString(TEXT("선택")));
    }
    else
    {
        eButtonStatus = LOBBY_SELECT_CHARACTER_BUTTON_STATUS::SELECTED;
        StatusTextBlock->SetText(FText::FromString(TEXT("선택됨")));
    }
}

void ULobbySelectCharacterButtonWidget::UpdateWidgetColor()
{
    switch (eButtonStatus)
    {
    case LOBBY_SELECT_CHARACTER_BUTTON_STATUS::DESELECTED:
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

        StatusTextBlock->SetColorAndOpacity(FLinearColor::White);

        break;
    case LOBBY_SELECT_CHARACTER_BUTTON_STATUS::SELECTED:
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



        break;
    default:
        break;
    }
}
