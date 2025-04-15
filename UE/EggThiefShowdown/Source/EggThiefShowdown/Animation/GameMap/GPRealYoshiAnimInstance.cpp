// Fill out your copyright notice in the Description page of Project Settings.


#include "GPRealYoshiAnimInstance.h"
#include "Components/StatusComponent/Game/RealYoshiStatusComponent.h"
#include "Actors/Game/NPC/RealYoshi.h"

UGPRealYoshiAnimInstance::UGPRealYoshiAnimInstance()
{
}

void UGPRealYoshiAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("UGPRealYoshiAnimInstance를 사용하려면 소유권자가 Pawn이여야 합니다."));
		return;
	}
	else if (!Pawn) { return; }

	ARealYoshi* RY = Cast<ARealYoshi>(Pawn);

	StatusComponent = RY->GetStatusComponent();
}

void UGPRealYoshiAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!StatusComponent) return;

	bIsIdle = StatusComponent->GetAnimStatus(GP_ANIM_BIT_IDLE);
	bIsWalk = StatusComponent->GetAnimStatus(GP_ANIM_BIT_WALK);
	bIsRun = StatusComponent->GetAnimStatus(GP_ANIM_BIT_RUN);
}
