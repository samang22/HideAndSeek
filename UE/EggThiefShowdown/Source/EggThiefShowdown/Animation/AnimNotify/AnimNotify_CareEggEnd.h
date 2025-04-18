// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CareEggEnd.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API UAnimNotify_CareEggEnd : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_CareEggEnd();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
