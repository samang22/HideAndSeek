// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_DamagedEnd.h"
#include "Actors/Game/Character/GamePlayer.h"
#include "Actors/Game/NPC/RealYoshi.h"

UAnimNotify_DamagedEnd::UAnimNotify_DamagedEnd()
{
}

void UAnimNotify_DamagedEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AGamePlayer* GP = Cast<AGamePlayer>(MeshComp->GetOwner()))
	{
		GP->SetCanMove(true);
		if (GP->GetIsEgg())
		{
			GP->SetEgg(nullptr);
		}
	}
	else if (ARealYoshi* RealYoshi = Cast<ARealYoshi>(MeshComp->GetOwner()))
	{
		RealYoshi->ResumeMovement();
	}
}
