// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "RealYoshiStatusComponent.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API URealYoshiStatusComponent : public UStatusComponent
{
	GENERATED_BODY()
	
public:
	URealYoshiStatusComponent();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_AnimationStatus)
	uint8 AnimationStatus;

public:
	void SetOnAnimationStatus(uint8 InBit);
	void SetOffAnimationStatus(uint8 InBit);
	UFUNCTION(Server, Reliable)
	void Server_SetOnAnimationStatus(uint8 InBit);
	UFUNCTION(Server, Reliable)
	void Server_SetOffAnimationStatus(uint8 InBit);

public:
	UFUNCTION()
	bool GetAnimStatus(uint8 InBit) const;

public:
	UFUNCTION()
	void OnRep_AnimationStatus();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
