// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ChannelBtnWidget.generated.h"

/**
 *
 */
UCLASS()
class NETWORK_API UChannelBtnWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetChannelInfo(const FString InIP, const int32 InPort, const int32 InMaxUserCount);
	UFUNCTION(BlueprintCallable)
	void SetCurrentUserCount(const int32 InCurrentUserCount);

	FString GetIP() const { return IP; }
	uint32 GetPort() const { return Port; }

protected:
	UFUNCTION()
	void OnChannelButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ChannelButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ChannelInfoTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxUserCountTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentUserCountTextBlock;

protected:
	FString IP;
	int32 CurrentUserCount = -1;
	int32 Port = -1;
	int32 MaxUserCount = -1;
};
