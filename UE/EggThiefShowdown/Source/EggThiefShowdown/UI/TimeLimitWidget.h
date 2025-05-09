// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeLimitWidget.generated.h"

class UImage;

UCLASS()
class EGGTHIEFSHOWDOWN_API UTimeLimitWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UTimeLimitWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MinuteTenImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MinuteOneImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SecondTenImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SecondOneImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ColonImage;


	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_0;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_1;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_2;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_3;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_4;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_5;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_6;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_7;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_8;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_9;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture2D_MarioFont_Colon;

public:
	void SetRemainTime(int32 MinuteTen, int32 MinuteOne, int32 SecondTen, int32 SecondOne);

};
