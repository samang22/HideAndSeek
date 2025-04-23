// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../StatusComponent.h"
#include "../../../Misc/Utils.h"
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

protected:
	LOBBY_CHARACTER_KIND eCharacterKind = LOBBY_CHARACTER_KIND::END;
public:
	void SetCharacterKind(LOBBY_CHARACTER_KIND eKind) { eCharacterKind = eKind; }
	LOBBY_CHARACTER_KIND GetCharacterKind() const { return eCharacterKind; }

public:
	UFUNCTION()
	void OnRep_AnimationStatus();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual bool CanMove() const override;
	void SetCanMove(bool _bFlag) { bCanMove = _bFlag; }
protected:
	bool bCanMove = true;

};
