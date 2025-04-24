// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DropEgg.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API UAnimNotify_DropEgg : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_DropEgg();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
