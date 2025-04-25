// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "HPStaminaWidget.generated.h"

class UProgressBar;
UCLASS()
class EGGTHIEFSHOWDOWN_API UHPStaminaWidget : public UUserWidget
{
	GENERATED_BODY()
	friend class AGamePlayer;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
public:
	void SetHPBarPercent(float InPercent);
	void SetStaminaBarPercent(float InPercent);

};
