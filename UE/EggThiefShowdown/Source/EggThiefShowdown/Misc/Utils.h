// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


#define PROBE_SIZE					5.0

enum class LOBBY_CHARACTER_ANIM_ENUM : uint8
{
	IDLE = 0,
	FISHING,
	PULLING,
	END,
};

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


enum class LOBBY_CHARACTER_ANIM_ENUM : uint8
{
	IDLE = 0,
	PICKED,
	UNPICKED,
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

