// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountdownWidget.generated.h"

class UImage;

UCLASS()
class EGGTHIEFSHOWDOWN_API UCountdownWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCountdownWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CountdownImage;

    // 카운트다운 텍스처 에셋들
    UPROPERTY()
    TObjectPtr<UTexture2D> Texture2D_MarioFont_5;

    UPROPERTY()
    TObjectPtr<UTexture2D> Texture2D_MarioFont_4;

    UPROPERTY()
    TObjectPtr<UTexture2D> Texture2D_MarioFont_3;

    UPROPERTY()
    TObjectPtr<UTexture2D> Texture2D_MarioFont_2;

    UPROPERTY()
    TObjectPtr<UTexture2D> Texture2D_MarioFont_1;

    UPROPERTY()
    TObjectPtr<UTexture2D> Texture2D_MarioFont_Start;
public:
	UFUNCTION()
	void SetCountdown(int32 Countdown);
};
