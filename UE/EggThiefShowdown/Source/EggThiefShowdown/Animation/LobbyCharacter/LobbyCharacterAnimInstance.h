// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LobbyCharacterAnimInstance.generated.h"


class ULobbyCharacterStatusComponent;

UCLASS()
class EGGTHIEFSHOWDOWN_API ULobbyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	const ULobbyCharacterStatusComponent* StatusComponent;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsPickedIdle = false;
	//UPROPERTY(BlueprintReadOnly)
	//bool bIsPicked = false;
	//UPROPERTY(BlueprintReadOnly)
	//bool bIsUnPicked = false;
};
