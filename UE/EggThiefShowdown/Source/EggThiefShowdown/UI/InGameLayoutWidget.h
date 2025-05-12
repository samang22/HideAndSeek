// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameLayoutWidget.generated.h"

class UHPStaminaWidget;
class UChatWidget;
class UEggGaugeWidget;
class UTimeLimitWidget;
class UCountdownWidget;
class UGameResultWidget;

UCLASS()
class EGGTHIEFSHOWDOWN_API UInGameLayoutWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHPStaminaWidget> HPStaminaWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UChatWidget> ChatWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEggGaugeWidget> EggGaugeWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTimeLimitWidget> TimeLimitWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCountdownWidget> CountdownWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGameResultWidget> GameResultWidget;

public:
	UHPStaminaWidget* GetHPStaminaWidget() { return HPStaminaWidget; }
	UChatWidget* GetChatWidget() { return ChatWidget; }
	UEggGaugeWidget* GetEggGaugeWidget() { return EggGaugeWidget; }
	UTimeLimitWidget* GetTimeLimitWidget() { return TimeLimitWidget; }
	UCountdownWidget* GetCountdownWidget() { return CountdownWidget; }
	UGameResultWidget* GetGameResultWidget() { return GameResultWidget; }
};
