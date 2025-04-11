// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "SoftWheelSpringArmComponent.generated.h"

#define MIN_TARGET_ARM_LENGTH		50.f
#define MAX_TARGET_ARM_LENGTH		800.f

UCLASS()
class EGGTHIEFSHOWDOWN_API USoftWheelSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetDesiredZoom(const float InValue);

	UFUNCTION(BlueprintCallable)
	void OnZoomWheel(const float ActionValue);

	UFUNCTION(BlueprintCallable)
	void SetMinMaxTargetArmLength(const float MinLength, const float MaxLength);

public:
	float GetMinTargetArmLength() const { return MinTargetArmLength; }
	float GetMaxTargetArmLength() const { return MaxTargetArmLength; }

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

protected:
	void UpdateDesiredTargetArmLength(const float DeltaTime);

protected:
	float MinTargetArmLength = MIN_TARGET_ARM_LENGTH;
	float MaxTargetArmLength = MAX_TARGET_ARM_LENGTH;

	float DesiredTargetArmLength = TargetArmLength;

	float WheelSpeed = 5.f;
};
