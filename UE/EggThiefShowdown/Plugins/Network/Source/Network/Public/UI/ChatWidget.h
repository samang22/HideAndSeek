// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "ChatWidget.generated.h"

USTRUCT()
struct FChatMessage
{
    GENERATED_USTRUCT_BODY()

    FString UserName;
    FString Message;

    FChatMessage() = default;
    FChatMessage(FString InUserName, FString InMessage) : UserName(InUserName), Message(InMessage) {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatSend, const FChatMessage&, ChatMessage);

UCLASS()
class NETWORK_API UChatWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION()
    virtual void AddChatMessage(const FChatMessage& NewMessage);

    FOnChatSend OnChatSend;

    void SetMyUserName(FString NewUserName) { MyUserName = NewUserName; }

protected:
    UFUNCTION()
    virtual void OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod);

    UFUNCTION()
    void OnSend();

protected:
    virtual void NativeConstruct() override;
    virtual void UpdateChatMessage();

    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

protected:
    UPROPERTY(meta = (BindWidget))
    UScrollBox* ChatScrollBox;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ChatTextBlock;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* ChatInputBox;

    UPROPERTY(meta = (BindWidget))
    UButton* SendButton;

protected:
    int32 MaxChatCount = 10;
    TArray<FChatMessage> ChatMessages;

    FString MyUserName;
};