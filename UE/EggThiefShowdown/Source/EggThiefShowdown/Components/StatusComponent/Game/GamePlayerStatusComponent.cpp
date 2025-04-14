// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerStatusComponent.h"
#include "../../../Misc/Utils.h"
#include "Net/UnrealNetwork.h"

UGamePlayerStatusComponent::UGamePlayerStatusComponent()
{
	SetIsReplicatedByDefault(true);
	SetOnAnimationStatus(GP_ANIM_BIT_IDLE);
}

void UGamePlayerStatusComponent::SetOnAnimationStatus(uint8 InBit)
{
	AnimationStatus |= InBit;
}

void UGamePlayerStatusComponent::SetOffAnimationStatus(uint8 InBit)
{
	AnimationStatus &= ~(InBit);
}

void UGamePlayerStatusComponent::Server_SetOnAnimationStatus_Implementation(uint8 InBit)
{
	SetOnAnimationStatus(InBit);
}

void UGamePlayerStatusComponent::Server_SetOffAnimationStatus_Implementation(uint8 InBit)
{
	SetOffAnimationStatus(InBit);
}

bool UGamePlayerStatusComponent::GetAnimStatus(uint8 InBit) const
{
	return AnimationStatus & InBit;
}

void UGamePlayerStatusComponent::OnRep_AnimationStatus()
{
	// do nothing
}

void UGamePlayerStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AnimationStatus);
}
