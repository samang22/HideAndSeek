// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TimeLimitWidget.h"
#include "Components/Image.h"

UTimeLimitWidget::UTimeLimitWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 

	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_0(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_0.MarioFont_0'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_1(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_1.MarioFont_1'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_2(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_2.MarioFont_2'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_3(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_3.MarioFont_3'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_4(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_4.MarioFont_4'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_5(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_5.MarioFont_5'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_6(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_6.MarioFont_6'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_7(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_7.MarioFont_7'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_8(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_8.MarioFont_8'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_9(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_9.MarioFont_9'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset_MarioFont_Colon(TEXT("/Script/Engine.Texture2D'/Game/Assets/Font/MarioFont_Colon.MarioFont_Colon'"));

	if (TextureAsset_MarioFont_0.Succeeded())
		Texture2D_MarioFont_0 = TextureAsset_MarioFont_0.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_0 failed to load"));

	if (TextureAsset_MarioFont_1.Succeeded())
		Texture2D_MarioFont_1 = TextureAsset_MarioFont_1.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_1 failed to load"));

	if (TextureAsset_MarioFont_2.Succeeded())
		Texture2D_MarioFont_2 = TextureAsset_MarioFont_2.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_2 failed to load"));

	if (TextureAsset_MarioFont_3.Succeeded())
		Texture2D_MarioFont_3 = TextureAsset_MarioFont_3.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_3 failed to load"));

	if (TextureAsset_MarioFont_4.Succeeded())
		Texture2D_MarioFont_4 = TextureAsset_MarioFont_4.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_4 failed to load"));

	if (TextureAsset_MarioFont_5.Succeeded())
		Texture2D_MarioFont_5 = TextureAsset_MarioFont_5.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_5 failed to load"));

	if (TextureAsset_MarioFont_6.Succeeded())
		Texture2D_MarioFont_6 = TextureAsset_MarioFont_6.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_6 failed to load"));

	if (TextureAsset_MarioFont_7.Succeeded())
		Texture2D_MarioFont_7 = TextureAsset_MarioFont_7.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_7 failed to load"));

	if (TextureAsset_MarioFont_8.Succeeded())
		Texture2D_MarioFont_8 = TextureAsset_MarioFont_8.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_8 failed to load"));

	if (TextureAsset_MarioFont_9.Succeeded())
		Texture2D_MarioFont_9 = TextureAsset_MarioFont_9.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_9 failed to load"));

	if (TextureAsset_MarioFont_Colon.Succeeded())
		Texture2D_MarioFont_Colon = TextureAsset_MarioFont_Colon.Object;
	else
		UE_LOG(LogTemp, Error, TEXT("TextureAsset_MarioFont_Colon failed to load"));

}

void UTimeLimitWidget::SetRemainTime(int32 MinuteTen, int32 MinuteOne, int32 SecondTen, int32 SecondOne)
{
	if (!MinuteTenImage || !MinuteOneImage || !SecondTenImage || !SecondOneImage || !ColonImage)
	{
		UE_LOG(LogTemp, Error, TEXT("UTimeLimitWidget::SetRemainTime // Image is null"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("TimeLimit %d%d:%d%d"), MinuteTen, MinuteOne, SecondTen, SecondOne);

	UTexture2D* MinuteTenTexture = nullptr;
	UTexture2D* MinuteOneTexture = nullptr;
	UTexture2D* SecondTenTexture = nullptr;
	UTexture2D* SecondOneTexture = nullptr;

	switch (MinuteTen)
	{
	case 0:
		MinuteTenTexture = Texture2D_MarioFont_0;
		break;
	case 1:
		MinuteTenTexture = Texture2D_MarioFont_1;
		break;
	case 2:
		MinuteTenTexture = Texture2D_MarioFont_2;
		break;
	case 3:
		MinuteTenTexture = Texture2D_MarioFont_3;
		break;
	case 4:
		MinuteTenTexture = Texture2D_MarioFont_4;
		break;
	case 5:
		MinuteTenTexture = Texture2D_MarioFont_5;
		break;
	case 6:
		MinuteTenTexture = Texture2D_MarioFont_6;
		break;
	case 7:
		MinuteTenTexture = Texture2D_MarioFont_7;
		break;
	case 8:
		MinuteTenTexture = Texture2D_MarioFont_8;
		break;
	case 9:
		MinuteTenTexture = Texture2D_MarioFont_9;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("UTimeLimitWidget::SetRemainTime // MinuteTen is out of Range"));
		return;
	}


	switch (MinuteOne)
	{
	case 0:
		MinuteOneTexture = Texture2D_MarioFont_0;
		break;
	case 1:
		MinuteOneTexture = Texture2D_MarioFont_1;
		break;
	case 2:
		MinuteOneTexture = Texture2D_MarioFont_2;
		break;
	case 3:
		MinuteOneTexture = Texture2D_MarioFont_3;
		break;
	case 4:
		MinuteOneTexture = Texture2D_MarioFont_4;
		break;
	case 5:
		MinuteOneTexture = Texture2D_MarioFont_5;
		break;
	case 6:
		MinuteOneTexture = Texture2D_MarioFont_6;
		break;
	case 7:
		MinuteOneTexture = Texture2D_MarioFont_7;
		break;
	case 8:
		MinuteOneTexture = Texture2D_MarioFont_8;
		break;
	case 9:
		MinuteOneTexture = Texture2D_MarioFont_9;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("UTimeLimitWidget::SetRemainTime // MinuteOne is out of Range"));
		return;
	}

	switch (SecondTen)
	{
	case 0:
		SecondTenTexture = Texture2D_MarioFont_0;
		break;
	case 1:
		SecondTenTexture = Texture2D_MarioFont_1;
		break;
	case 2:
		SecondTenTexture = Texture2D_MarioFont_2;
		break;
	case 3:
		SecondTenTexture = Texture2D_MarioFont_3;
		break;
	case 4:
		SecondTenTexture = Texture2D_MarioFont_4;
		break;
	case 5:
		SecondTenTexture = Texture2D_MarioFont_5;
		break;
	case 6:
		SecondTenTexture = Texture2D_MarioFont_6;
		break;
	case 7:
		SecondTenTexture = Texture2D_MarioFont_7;
		break;
	case 8:
		SecondTenTexture = Texture2D_MarioFont_8;
		break;
	case 9:
		SecondTenTexture = Texture2D_MarioFont_9;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("UTimeLimitWidget::SetRemainTime // SecondTen is out of Range"));
		return;
	}

	switch (SecondOne)
	{
	case 0:
		SecondOneTexture = Texture2D_MarioFont_0;
		break;
	case 1:
		SecondOneTexture = Texture2D_MarioFont_1;
		break;
	case 2:
		SecondOneTexture = Texture2D_MarioFont_2;
		break;
	case 3:
		SecondOneTexture = Texture2D_MarioFont_3;
		break;
	case 4:
		SecondOneTexture = Texture2D_MarioFont_4;
		break;
	case 5:
		SecondOneTexture = Texture2D_MarioFont_5;
		break;
	case 6:
		SecondOneTexture = Texture2D_MarioFont_6;
		break;
	case 7:
		SecondOneTexture = Texture2D_MarioFont_7;
		break;
	case 8:
		SecondOneTexture = Texture2D_MarioFont_8;
		break;
	case 9:
		SecondOneTexture = Texture2D_MarioFont_9;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("UTimeLimitWidget::SetRemainTime // SecondOne is out of Range"));
		return;
	}

	MinuteTenImage->SetBrushFromTexture(MinuteTenTexture);
	FVector2D MinuteTenTextureSize(MinuteTenTexture->GetSizeX(), MinuteTenTexture->GetSizeY());
	MinuteTenImage->SetBrushSize(MinuteTenTextureSize);
	MinuteTenImage->ForceLayoutPrepass(); // 즉시 적용

	MinuteOneImage->SetBrushFromTexture(MinuteOneTexture);
	FVector2D MinuteOneTextureSize(MinuteOneTexture->GetSizeX(), MinuteOneTexture->GetSizeY());
	MinuteOneImage->SetBrushSize(MinuteOneTextureSize);
	MinuteOneImage->ForceLayoutPrepass(); // 즉시 적용

	SecondTenImage->SetBrushFromTexture(SecondTenTexture);
	FVector2D SecondTenTextureSize(SecondTenTexture->GetSizeX(), SecondTenTexture->GetSizeY());
	SecondTenImage->SetBrushSize(SecondTenTextureSize);
	SecondTenImage->ForceLayoutPrepass(); // 즉시 적용

	SecondOneImage->SetBrushFromTexture(SecondOneTexture);
	FVector2D SecondOneTextureSize(SecondOneTexture->GetSizeX(), SecondOneTexture->GetSizeY());
	SecondOneImage->SetBrushSize(SecondOneTextureSize);
	SecondOneImage->ForceLayoutPrepass(); // 즉시 적용

	ColonImage->SetBrushFromTexture(Texture2D_MarioFont_Colon);
	FVector2D ColonTextureSize(Texture2D_MarioFont_Colon->GetSizeX(), Texture2D_MarioFont_Colon->GetSizeY());
	ColonImage->SetBrushSize(ColonTextureSize);
	ColonImage->ForceLayoutPrepass(); // 즉시 적용

	
}

