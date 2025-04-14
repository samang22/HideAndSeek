// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MarioAttack.h"
#include "Actors/Game/Character/GamePlayer.h"

UAnimNotify_MarioAttack::UAnimNotify_MarioAttack()
{
}

void UAnimNotify_MarioAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
#if WITH_EDITOR
	if (GIsEditor && MeshComp->GetWorld() != GWorld) { return; } // 에디터 프리뷰
#endif


	if (AGamePlayer* GP = Cast<AGamePlayer>(MeshComp->GetOwner()))
	{
		FVector Location = GP->GetActorLocation();
	}
}
