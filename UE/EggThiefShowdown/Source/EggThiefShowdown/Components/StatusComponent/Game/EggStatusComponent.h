// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Misc/Utils.h"
#include "EggStatusComponent.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API UEggStatusComponent : public UStatusComponent
{
	GENERATED_BODY()
	
public:
	UEggStatusComponent();

protected:
	float CurrentCoolTime = 0.f;
public:
	void SetCoolTimeZero() { CurrentCoolTime = 0.f; }
	bool IsCoolTime() { return EGG_MAX_COOLTIME > CurrentCoolTime; }


protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

protected:
	bool bCanMove = true;
public:
	void SetCanMove(bool bFlag) { bCanMove = bFlag;	}

};
