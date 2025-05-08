// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CountdownWidget.h"

UCountdownWidget::UCountdownWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_5(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_5.MarioFont_5'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_4(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_4.MarioFont_4'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_3(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_3.MarioFont_3'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_2(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_2.MarioFont_2'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_1(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_1.MarioFont_1'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_Start(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_Start.MarioFont_Start'"));

	if (TextureAsset_MarioFont_5.Succeeded())
		Texture2D_MarioFont_5 = TextureAsset_MarioFont_5.Object;
    else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_5 failed to load"));

    if (TextureAsset_MarioFont_4.Succeeded())
		Texture2D_MarioFont_4 = TextureAsset_MarioFont_4.Object;
    else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_4 failed to load"));

	if (TextureAsset_MarioFont_3.Succeeded())
		Texture2D_MarioFont_3 = TextureAsset_MarioFont_3.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_3 failed to load"));

	if (TextureAsset_MarioFont_2.Succeeded())
		Texture2D_MarioFont_2 = TextureAsset_MarioFont_2.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_2 failed to load"));

	if (TextureAsset_MarioFont_1.Succeeded())
		Texture2D_MarioFont_1 = TextureAsset_MarioFont_1.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_1 failed to load"));

	if (TextureAsset_MarioFont_Start.Succeeded())
		Texture2D_MarioFont_Start = TextureAsset_MarioFont_Start.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_Start failed to load"));
}

void UCountdownWidget::SetCountdown(int32 Countdown)
{
    if (!CountdownImage)
    {
        UE_LOG(LogTemp, Warning, TEXT("CountdownImage is null"));
        check(false);
        return;
    }

	UE_LOG(LogTemp, Warning, TEXT("Countdown : %d"), Countdown);

    UTexture2D* SelectedTexture = nullptr;

	switch (Countdown)
    {
    case 6:
        SelectedTexture = Texture2D_MarioFont_5;
        break;
    case 5:
        SelectedTexture = Texture2D_MarioFont_4;
        break;
    case 4:
        SelectedTexture = Texture2D_MarioFont_3;
        break;
    case 3:
        SelectedTexture = Texture2D_MarioFont_2;
        break;
    case 2:
        SelectedTexture = Texture2D_MarioFont_1;
        break;
    case 1:
        SelectedTexture = Texture2D_MarioFont_Start;
        break;
    default:
        CountdownImage->SetBrushFromTexture(nullptr);
        return;
    }

    if (SelectedTexture)
    {
        // 이미지 설정
        CountdownImage->SetBrushFromTexture(SelectedTexture);

        // 이미지 크기를 텍스처 크기에 맞게 설정
        FVector2D TextureSize(SelectedTexture->GetSizeX(), SelectedTexture->GetSizeY());
        UE_LOG(LogTemp, Warning, TEXT("TextureSize : %d, %d"), SelectedTexture->GetSizeX(), SelectedTexture->GetSizeY());
        CountdownImage->SetBrushSize(TextureSize);
    }
}
