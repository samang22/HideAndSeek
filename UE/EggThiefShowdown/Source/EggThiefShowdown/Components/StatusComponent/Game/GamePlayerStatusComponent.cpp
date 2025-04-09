// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerStatusComponent.h"
#include "../../../Misc/Utils.h"

UGamePlayerStatusComponent::UGamePlayerStatusComponent()
{
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

bool UGamePlayerStatusComponent::GetAnimStatus(uint8 InBit) const
{
	return false;
}
