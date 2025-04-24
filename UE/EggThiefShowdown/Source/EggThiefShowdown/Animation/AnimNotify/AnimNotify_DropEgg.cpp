// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_DropEgg.h"
#include "Actors/Game/Character/GamePlayer.h"
#include "Misc/Utils.h"

UAnimNotify_DropEgg::UAnimNotify_DropEgg()
{
}

void UAnimNotify_DropEgg::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
#if WITH_EDITOR
	if (GIsEditor && MeshComp->GetWorld() != GWorld) { return; } // 에디터 프리뷰
#endif

	if (AGamePlayer* GP = Cast<AGamePlayer>(MeshComp->GetOwner()))
	{
		if (GP->GetIsEgg())
		{
			GP->DropEgg();
		}
	}
}
