// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_DamagedEnd.h"
#include "Actors/Game/Character/GamePlayer.h"


UAnimNotify_DamagedEnd::UAnimNotify_DamagedEnd()
{
}

void UAnimNotify_DamagedEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AGamePlayer* GP = Cast<AGamePlayer>(MeshComp->GetOwner()))
	{
		GP->SetMovement(true);
	}
}
