// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChannelListWidget.h"
#include "Subsystem/HaServerSubsystem.h"
#include "Channel/LoginChannel.h"
#include "UI/ChannelBtnWidget.h"

void UChannelListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HaServerSubsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();
	LoginChannel = HaServerSubsystem->GetLoginChannel();

	ChannelVerticalBox->ClearChildren();
}

void UChannelListWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!LoginChannel)
	{
		LoginChannel = HaServerSubsystem->GetLoginChannel();
		if (!LoginChannel)
		{
			return;
		}
	}

	if (LoginID->GetText().ToString() != LoginChannel->GetLoginUserName())
	{
		LoginID->SetText(FText::FromString(LoginChannel->GetLoginUserName()));
	}

	CreateChannelBtnWidget();
	UpdateChannelInfos();

	static const float Interval = 1.f;
	static float AccDT = Interval;
	AccDT += InDeltaTime;
	if (AccDT >= Interval)
	{
		AccDT -= Interval;

		HaServerSubsystem->RequestDediServerInfo();
	}
}

void UChannelListWidget::CreateChannelBtnWidget()
{
	int32 ServerCount = LoginChannel->GetServerCount();

	const int32 ChildrenCount = ChannelVerticalBox->GetChildrenCount();
	if (ChildrenCount != ServerCount)
	{
		
		UClass* BtnWidgetClass = LoadClass<UChannelBtnWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Channel/UI_ChannelBtnWidget.UI_ChannelBtnWidget_C'"));
		check(BtnWidgetClass);
		
		for (int32 i = 0; i < ServerCount - ChildrenCount; ++i)
		{
			UChannelBtnWidget* NewChannelBtnWidget = CreateWidget<UChannelBtnWidget>(GetWorld(), BtnWidgetClass);
			ChannelVerticalBox->AddChildToVerticalBox(NewChannelBtnWidget);
		}
	}
}

void UChannelListWidget::UpdateChannelInfos()
{
	const TArray<FDediServerInfo>& DediServerInfo = LoginChannel->GetDediServerInfos();
	TArray<UWidget*> Childrens = ChannelVerticalBox->GetAllChildren();
	for (int32 i = 0; i < DediServerInfo.Num(); ++i)
	{
		UChannelBtnWidget* ChannelBtn = Cast<UChannelBtnWidget>(Childrens[i]);
		if (ChannelBtn)
		{
			ChannelBtn->SetChannelInfo(DediServerInfo[i].IP, DediServerInfo[i].Port, DediServerInfo[i].MaxPlayers);
			ChannelBtn->SetCurrentUserCount(DediServerInfo[i].CurrentPlayers);
		}
	}
}
