// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameResultWidget.generated.h"

class UImage;

UCLASS()
class EGGTHIEFSHOWDOWN_API UGameResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UGameResultWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> GameResultImage;

    UPROPERTY()
    TObjectPtr<UTexture2D> Texture2D_Win;
    UPROPERTY()
    TObjectPtr<UTexture2D> Texture2D_Lose;

public:
    UFUNCTION()
    void SetGameResult(bool bResult);
};
