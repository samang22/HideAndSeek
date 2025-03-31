// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "ChannelListWidget.generated.h"

UCLASS()
class NETWORK_API UChannelListWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void CreateChannelBtnWidget();
	void UpdateChannelInfos();

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ChannelVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoginID;

protected:
	class UHaServerSubsystem* HaServerSubsystem = nullptr;
	class ULoginChannel* LoginChannel = nullptr;
};
