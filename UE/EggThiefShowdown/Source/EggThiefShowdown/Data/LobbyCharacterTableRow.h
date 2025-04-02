// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Utils.h"
#include "Animation/NPCAnimInstance.h"
#include "AIController.h"

#include "FishTableRow.generated.h"

USTRUCT()
struct DREAMINGISLAND_API FFishTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Fish")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Fish")
	FTransform MeshTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, Category = "Fish|Animation")
	TSubclassOf<UAnimInstance> AnimClass;

public:
	UPROPERTY(EditAnywhere, Category = "Fish|Collision")
	float CollisionSphereRadius = 32.f;

public: // To sense Lure
	UPROPERTY(EditAnywhere, Category = "Fish|SenseLureCollision")
	float SenseLureCollisionSphereRadius = 32.f;

public: // Movement
	UPROPERTY(EditAnywhere, Category = "Fish|Movement")
	float MovementMaxSpeed = 100.f;
	UPROPERTY(EditAnywhere, Category = "Fish|Movement")
	float RushMovementMaxSpeed = 200.f;



public: // Animation
	UPROPERTY(EditAnywhere, Category = "Fish|Animation")
	UAnimMontage* PokeMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Fish|Animation")
	UAnimMontage* TurnMontage = nullptr;

public: // AI
	UPROPERTY(EditAnywhere, Category = "Fish|AI")
	TSubclassOf<AAIController> AIControllerClass;
};
