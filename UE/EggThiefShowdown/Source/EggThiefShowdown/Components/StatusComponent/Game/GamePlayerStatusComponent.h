// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../StatusComponent.h"
#include "GamePlayerStatusComponent.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API UGamePlayerStatusComponent : public UStatusComponent
{
	GENERATED_BODY()
	
public:
	UGamePlayerStatusComponent();

protected:
	UPROPERTY()
	uint8 AnimationStatus;

public:
	void SetOnAnimationStatus(uint8 InBit);
	void SetOffAnimationStatus(uint8 InBit);

public:
	UFUNCTION()
	bool GetAnimStatus(uint8 InBit) const;
};
