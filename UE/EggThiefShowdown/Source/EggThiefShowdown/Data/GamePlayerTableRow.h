// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Misc/Utils.h"
#include "GamePlayerTableRow.generated.h"

USTRUCT()
struct EGGTHIEFSHOWDOWN_API FGamePlayerTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "GamePlayer")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "GamePlayer")
	FTransform MeshTransform = FTransform::Identity;

public: // Animation
	UPROPERTY(EditAnywhere, Category = "GamePlayer|Animation")
	TSubclassOf<UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, Category = "GamePlayer|Animation")
	UAnimMontage* PickupMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "GamePlayer|Animation")
	UAnimMontage* DamagedMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "GamePlayer|Animation")
	UAnimMontage* AttackMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "GamePlayer|Animation")
	UAnimMontage* DropEggMontage = nullptr;

public: // For RealYoshi
	UPROPERTY(EditAnywhere, Category = "AI")
	float CollisionSphereRadius = 32.f;
};
