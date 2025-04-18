// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_CareEggEnd.h"
#include "Actors/Game/NPC/RealYoshi.h"
UAnimNotify_CareEggEnd::UAnimNotify_CareEggEnd()
{
}

void UAnimNotify_CareEggEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
#if WITH_EDITOR
	if (GIsEditor && MeshComp->GetWorld() != GWorld) { return; } // 에디터 프리뷰
#endif
	
	if (ARealYoshi* RY = Cast<ARealYoshi>(MeshComp->GetOwner()))
	{
		RY->ResumeMovement();
	}
}
