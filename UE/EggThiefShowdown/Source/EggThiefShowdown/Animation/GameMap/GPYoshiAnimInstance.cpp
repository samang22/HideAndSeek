// Fill out your copyright notice in the Description page of Project Settings.


#include "GPYoshiAnimInstance.h"
#include "../../Components/StatusComponent/Game/GamePlayerStatusComponent.h"
#include "../../Actors/Game/Character/GamePlayer.h"

UGPYoshiAnimInstance::UGPYoshiAnimInstance()
{
}

void UGPYoshiAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("UGPYoshiAnimInstance를 사용하려면 소유권자가 Pawn이여야 합니다."));
		return;
	}
	else if (!Pawn) { return; }

	AGamePlayer* GP = Cast<AGamePlayer>(Pawn);

	StatusComponent = GP->GetStatusComponent();
}

void UGPYoshiAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!StatusComponent) return;

	bIsIdle = StatusComponent->GetAnimStatus(GP_ANIM_BIT_IDLE);
	bIsWalk = StatusComponent->GetAnimStatus(GP_ANIM_BIT_WALK);
	bIsRun = StatusComponent->GetAnimStatus(GP_ANIM_BIT_RUN);
}
