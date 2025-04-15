// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusComponent/Game/RealYoshiStatusComponent.h"
#include "Misc/Utils.h"
#include "Net/UnrealNetwork.h"

URealYoshiStatusComponent::URealYoshiStatusComponent()
{
	SetIsReplicatedByDefault(true);
	SetOnAnimationStatus(GP_ANIM_BIT_IDLE);
}

void URealYoshiStatusComponent::SetOnAnimationStatus(uint8 InBit)
{
	AnimationStatus |= InBit;
}

void URealYoshiStatusComponent::SetOffAnimationStatus(uint8 InBit)
{
	AnimationStatus &= ~(InBit);
}

void URealYoshiStatusComponent::Server_SetOnAnimationStatus_Implementation(uint8 InBit)
{
	SetOnAnimationStatus(InBit);
}

void URealYoshiStatusComponent::Server_SetOffAnimationStatus_Implementation(uint8 InBit)
{
	SetOffAnimationStatus(InBit);
}

bool URealYoshiStatusComponent::GetAnimStatus(uint8 InBit) const
{
	return AnimationStatus & InBit;
}

void URealYoshiStatusComponent::OnRep_AnimationStatus()
{
	// do nothing
}

void URealYoshiStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AnimationStatus);
}
