// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MarioPickup.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API UAnimNotify_MarioPickup : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_MarioPickup();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
