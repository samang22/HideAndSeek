// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GPRealYoshiAnimInstance.generated.h"

class URealYoshiStatusComponent;

UCLASS()
class EGGTHIEFSHOWDOWN_API UGPRealYoshiAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UGPRealYoshiAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	URealYoshiStatusComponent* StatusComponent = nullptr;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsWalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsRun = false;
};
