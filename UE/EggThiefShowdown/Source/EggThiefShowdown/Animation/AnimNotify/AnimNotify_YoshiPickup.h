// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_YoshiPickup.generated.h"


UCLASS()
class EGGTHIEFSHOWDOWN_API UAnimNotify_YoshiPickup : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_YoshiPickup();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
