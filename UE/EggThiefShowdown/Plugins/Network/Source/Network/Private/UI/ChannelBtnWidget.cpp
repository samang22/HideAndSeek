// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChannelBtnWidget.h"
#include "Subsystem/HaServerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UChannelBtnWidget::SetChannelInfo(const FString InIP, const int32 InPort, const int32 InMaxUserCount)
{
	if (IP == InIP && Port == InPort && MaxUserCount == InMaxUserCount)
	{
		return;
	}

	if (InIP.IsEmpty() || InPort <= 0 || InMaxUserCount <= 0)
	{
		return;
	}

	IP = InIP;
	Port = InPort;
	MaxUserCount = InMaxUserCount;

	ChannelButton->OnClicked.Clear();
	ChannelButton->OnClicked.AddDynamic(this, &UChannelBtnWidget::OnChannelButtonClicked);

	CurrentUserCountTextBlock->SetText(FText::FromString(TEXT("?")));
	MaxUserCountTextBlock->SetText(FText::FromString(FString::FromInt(InMaxUserCount)));

	FString ChannelInfoString = IP + TEXT(":") + FString::FromInt(Port);
	ChannelInfoTextBlock->SetText(FText::FromString(ChannelInfoString));
}

void UChannelBtnWidget::SetCurrentUserCount(const int32 InCurrentUserCount)
{
	CurrentUserCount = InCurrentUserCount;
	CurrentUserCountTextBlock->SetText(FText::FromString(FString::FromInt(CurrentUserCount)));
}

void UChannelBtnWidget::OnChannelButtonClicked()
{
	UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();
	const FString URL = IP + TEXT(":") + FString::FromInt(Port);

	const FString ID = Subsystem->GetLoginUserName();
	const FString Password = Subsystem->GetLoginPassword();
	const FString Options = TEXT("ID=") + ID + TEXT("?Password=") + Password;
	UGameplayStatics::OpenLevel(this, FName(URL), true, Options);
}
