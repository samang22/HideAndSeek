// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChatWidget.h"
#include "Input/Events.h"

void UChatWidget::NativeConstruct()
{
    Super::NativeConstruct();
    SetIsFocusable(true);

    SendButton->OnClicked.AddDynamic(this, &ThisClass::OnSend);
    OnChatSend.AddDynamic(this, &ThisClass::AddChatMessage);

    ChatInputBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatCommitted);
    ChatInputBox->SetText(FText::FromString(TEXT("")));
    ChatTextBlock->SetText(FText::FromString(TEXT("")));

    SetMyUserName(TEXT("Test"));
}

void UChatWidget::AddChatMessage(const FChatMessage& NewMessage)
{
    if (ChatMessages.Num() >= MaxChatCount)
    {
        ChatMessages.RemoveAt(0);
    }

    ChatMessages.Emplace(NewMessage);
    UpdateChatMessage();
}

void UChatWidget::UpdateChatMessage()
{
    FString CombinedText;
    for (const FChatMessage& ChatMessage : ChatMessages)
    {
        CombinedText.Append(ChatMessage.UserName + ": " + ChatMessage.Message + "\n");
    }

    ChatTextBlock->SetText(FText::FromString(CombinedText));
    ChatScrollBox->ScrollToEnd();
}

void UChatWidget::OnSend()
{
    FText InputText = ChatInputBox->GetText();
    if (InputText.IsEmpty()) { return; }

    ChatInputBox->SetText(FText::FromString(TEXT("")));

    FChatMessage NewChatMessage = FChatMessage(MyUserName, InputText.ToString());
    OnChatSend.Broadcast(NewChatMessage);
}

FReply UChatWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Up)
    {
        for (auto It = ChatMessages.rbegin(); It != ChatMessages.rend(); ++It)
        {
            if ((*It).UserName == MyUserName)
            {
                ChatInputBox->SetText(FText::FromString((*It).Message));
                break;
            }
        }
        return FReply::Handled();
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UChatWidget::OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (ETextCommit::OnEnter == CommitMethod)
    {
        OnSend();
    }
}