// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Misc/Utils.h"

#include "LobbyCharacterTableRow.generated.h"

USTRUCT()
struct EGGTHIEFSHOWDOWN_API FLobbyCharacterTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	FTransform MeshTransform = FTransform::Identity;

public: // Animation
	UPROPERTY(EditAnywhere, Category = "LobbyCharacter|Animation")
	TSubclassOf<UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter|Animation")
	UAnimMontage* IdleMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "LobbyCharacter|Animation")
	UAnimMontage* PickedTurnMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "LobbyCharacter|Animation")
	UAnimMontage* UnPickedTurnMontage = nullptr;
};
