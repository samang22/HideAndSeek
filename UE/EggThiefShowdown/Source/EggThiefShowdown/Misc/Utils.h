// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Defines.h"



enum class LOBBY_CHARACTER_ANIM_ENUM : uint8
{
	IDLE = 0,
	PICKEDIDLE,
	END,
};

enum class LOBBY_CHARACTER_STATUS : uint8
{
	IDLE = 0,
	PICKED,
	PICKEDIDLE,
	UNPICKED,
	END,
};

enum class LOBBY_CHARACTER_MONTAGE : uint8
{
	PICKED = 0,
	UNPICKED,
	END,
};

UENUM(BlueprintType)
enum class LOBBY_CHARACTER_SELECT_BUTTON_STATE : uint8
{
	EMPTY = 0,
	OTHER,
	MY,
	END,
};

UENUM(BlueprintType)
enum class LOBBY_CHARACTER_KIND : uint8
{
	MARIO = 0,
	YOSHI,
	END,
};


enum class YOSHI_MONTAGE : uint8
{
	DAMAGED = 0,
	PICKUP,
	END,
};

enum class GAME_PLAYER_MONTAGE : uint8
{
	DAMAGED = 0,
	PICKUP,
	END,
};

enum class GAME_PLAYER_ANIM_STATE : uint8
{
	IDLE = 0,
	WALK,
	RUN,
};



namespace CollisionProfileName
{
	static inline FName Yoshi = TEXT("Yoshi");
	static inline FName Egg = TEXT("Egg");
	static inline FName Mario = TEXT("Mario");
	static inline FName MapMesh = TEXT("MapMesh");
	static inline FName MarioAttack = TEXT("MarioAttack");
	static inline FName YoshiPickup = TEXT("YoshiPickup");
}


class FUtils
{
public:
	static const UInputAction* GetInputActionFromName(UInputMappingContext* IMC, const FName& InName)
	{
		const TArray<FEnhancedActionKeyMapping>& Mappings = IMC->GetMappings();
		for (auto& It : Mappings)
		{
			if (It.Action->GetFName() == InName)
			{
				return It.Action.Get();
			}
		}

		return nullptr;
	}
};



inline bool IsNearlyEqualVector_My(const FVector& A, const FVector& B, float Tolerance = KINDA_SMALL_NUMBER)
{
	return FVector::DistSquared(A, B) < FMath::Square(Tolerance);
}

inline float CalculateAngleBetweenVectors(const FVector& A, const FVector& B) 
{
	// Calculate dot product
	float DotProduct = FVector::DotProduct(A, B);

	// 두 벡터의 크기를 계산
	float MagnitudeA = A.Size();
	float MagnitudeB = B.Size();

	// 코사인 값 계산
	float CosTheta = DotProduct / (MagnitudeA * MagnitudeB);

	// 각도 계산 (라디안)
	float AngleRadians = FMath::Acos(CosTheta);

	// 라디안을 각도로 변환
	return FMath::RadiansToDegrees(AngleRadians);
}

inline FRotator MyGetRotator(FVector Direction)
{
	double Pitch = FMath::Asin(Direction.Z) * (180.0f / PI);
	double Yaw = FMath::Atan2(Direction.Y, Direction.X) * (180.0f / PI);
	return (FRotator(Pitch, Yaw, 0.0));
}

