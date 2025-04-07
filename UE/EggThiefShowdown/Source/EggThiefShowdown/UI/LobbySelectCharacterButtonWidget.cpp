// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySelectCharacterButtonWidget.h"
#include "Subsystem/HaServerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/HaServerSubsystem.h"
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
    //UpdateWidgetState();
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


void ULobbySelectCharacterButtonWidget::UpdateWidgetState()
{
    UE_LOG(LogTemp, Warning, TEXT("UpdateWidgetState called"));

    UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();

    if (Subsystem)
    {
        const FString LoginUserName = Subsystem->GetLoginUserName();

        if (LoginUserName == UserName)
        {
            UE_LOG(LogTemp, Warning, TEXT("LoginUserName == UserName"));

            switch (eLobbyCharacterEnum)
            {
            case LOBBY_CHARACTER_KIND::MARIO:
                SetButtonColor(FLinearColor::Red);
                break;
            case LOBBY_CHARACTER_KIND::YOSHI:
                SetButtonColor(FLinearColor::Green);
                break;
            default:
                break;
            }

            StatusTextBlock->SetColorAndOpacity(FLinearColor::White);
            StatusTextBlock->SetText(FText::FromString(UserName));
        }
        else if (UserName.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("UserName.IsEmpty()"));

            switch (eLobbyCharacterEnum)
            {
            case LOBBY_CHARACTER_KIND::MARIO:
                SetButtonColor(FLinearColor::Red);
                break;
            case LOBBY_CHARACTER_KIND::YOSHI:
                SetButtonColor(FLinearColor::Green);
                break;
            default:
                break;
            }
            StatusTextBlock->SetColorAndOpacity(FLinearColor::White);
            StatusTextBlock->SetText(FText::FromString(TEXT("선택")));
        }
        else // LoginUserName != UserName
        {
            UE_LOG(LogTemp, Warning, TEXT("LoginUserName != UserName"));

            SetButtonColor(FLinearColor::Gray);

            switch (eLobbyCharacterEnum)
            {
            case LOBBY_CHARACTER_KIND::MARIO:
                StatusTextBlock->SetColorAndOpacity(FLinearColor::Red);
                break;
            case LOBBY_CHARACTER_KIND::YOSHI:
                StatusTextBlock->SetColorAndOpacity(FLinearColor::Green);
                break;
            default:
                break;
            }
            StatusTextBlock->SetText(FText::FromString(UserName));
        }

    }
}
