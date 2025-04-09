// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../../Misc/Utils.h"
#include "GPYoshiAnimInstance.generated.h"

class UGamePlayerStatusComponent;

UCLASS()
class EGGTHIEFSHOWDOWN_API UGPYoshiAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UGPYoshiAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	UGamePlayerStatusComponent* StatusComponent = nullptr;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsIdle = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsWalk = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsRun = false;
};
