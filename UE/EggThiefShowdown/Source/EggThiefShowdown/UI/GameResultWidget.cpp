// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameResultWidget.h"
#include "Components/Image.h"

UGameResultWidget::UGameResultWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_Win(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_Win.MarioFont_Win'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_Lose(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_Lose.MarioFont_Lose'"));

	if (TextureAsset_Win.Succeeded())
		Texture2D_Win = TextureAsset_Win.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_Win failed to load"));

	if (TextureAsset_Win.Succeeded())
		Texture2D_Lose = TextureAsset_Lose.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("Texture2D_Lose failed to load"));

}

void UGameResultWidget::SetGameResult(bool bResult)
{
	if (!GameResultImage)
	{
		UE_LOG(LogTemp, Error, TEXT("GameResultImage is null"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("GameResult : %d"), bResult);
	UTexture2D* SelectedTexture = nullptr;
	if (bResult)
	{
		SelectedTexture = Texture2D_Win;
	}
	else
	{
		SelectedTexture = Texture2D_Lose;
	}

	if (SelectedTexture)
	{
		GameResultImage->SetBrushFromTexture(SelectedTexture);

		// 이미지 크기를 텍스처 크기에 맞게 설정
		FVector2D TextureSize(SelectedTexture->GetSizeX(), SelectedTexture->GetSizeY());
		GameResultImage->SetBrushSize(TextureSize);
		GameResultImage->ForceLayoutPrepass();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UGameResultWidget::SetGameResult // SelectedTexture is null"));
	}

}
