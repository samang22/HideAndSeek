// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerStatusComponent.h"
#include "../../../Misc/Utils.h"
#include "Net/UnrealNetwork.h"

UGamePlayerStatusComponent::UGamePlayerStatusComponent()
{
	SetIsReplicatedByDefault(true);
	SetOnAnimationStatus(GP_ANIM_BIT_IDLE);
}

void UGamePlayerStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetAnimStatus(GP_ANIM_BIT_RUN))
	{
		ConsumeStamina(DeltaTime);
	}
	else
	{
		RegenStamina(DeltaTime);
	}

	if (!CanSprint())
	{
		SetExhausted();
		Server_SetOffAnimationStatus(GP_ANIM_BIT_RUN);
		Server_SetOffAnimationStatus(GP_ANIM_BIT_WALK);
	}

	ExhaustedTime += DeltaTime;
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
	UE_LOG(LogTemp, Warning, TEXT("Server_SetOnAnimationStatus_Implementation"));
}

void UGamePlayerStatusComponent::Server_SetOffAnimationStatus_Implementation(uint8 InBit)
{
	SetOffAnimationStatus(InBit);
	//UE_LOG(LogTemp, Warning, TEXT("Server_SetOffAnimationStatus_Implementation"));
}

bool UGamePlayerStatusComponent::GetAnimStatus(uint8 InBit) const
{
	return AnimationStatus & InBit;
}

void UGamePlayerStatusComponent::OnRep_AnimationStatus()
{
	// do nothing
}

void UGamePlayerStatusComponent::OnRep_CurrentStamina()
{
}

void UGamePlayerStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AnimationStatus);
	DOREPLIFETIME(ThisClass, MaxStamina);
	DOREPLIFETIME(ThisClass, CurrentStamina);
	DOREPLIFETIME(ThisClass, ExhaustedTime);
}

bool UGamePlayerStatusComponent::CanMove() const
{
	bool bSuperCanMove = Super::CanMove();
	if (!bSuperCanMove) return false;

	return bCanMove;
}

void UGamePlayerStatusComponent::ConsumeStamina(float InDeltaTime)
{
	if (!GetOwner()->HasAuthority()) return;

	// 스태미나 소모
	if (CurrentStamina > 0)
	{
		CurrentStamina -= StaminaConsumeRate * InDeltaTime;
		CurrentStamina = FMath::Max(CurrentStamina, 0);
	}
}

void UGamePlayerStatusComponent::RegenStamina(float InDeltaTime)
{
	if (!GetOwner()->HasAuthority()) return;

	// 스태미나 회복
	if (CurrentStamina < MaxStamina)
	{
		CurrentStamina += StaminaRegenRate * InDeltaTime;
		CurrentStamina = FMath::Min(CurrentStamina, MaxStamina);
	}
}

void UGamePlayerStatusComponent::SetExtraInfoWithCharacterKind()
{
	if (!GetOwner()->HasAuthority()) return;

	switch (eCharacterKind)
	{
	case LOBBY_CHARACTER_KIND::MARIO:
		MaxStamina = MARIO_MAX_STAMINA;
		CurrentStamina = MaxStamina;
		StaminaRegenRate = MARIO_STAMINA_REGEN;
		StaminaConsumeRate = MARIO_STAMINA_CONSUME;
		SetMaxHP(MARIO_MAX_HP);
		SetCurrentHP(MARIO_MAX_HP);
		break;
	case LOBBY_CHARACTER_KIND::YOSHI:
		MaxStamina = YOSHI_MAX_STAMINA;
		CurrentStamina = MaxStamina;
		StaminaRegenRate = YOSHI_STAMINA_REGEN;
		StaminaConsumeRate = YOSHI_STAMINA_CONSUME;
		SetMaxHP(YOSHI_MAX_HP);
		SetCurrentHP(YOSHI_MAX_HP);
		break;
	default:
		check(false);
		break;
	}
}

bool UGamePlayerStatusComponent::CanSprint()
{
	bool bFlag = CurrentStamina > 0.016f * StaminaConsumeRate * 10.f;
	return bFlag;
}

void UGamePlayerStatusComponent::SetExhausted()
{
	ExhaustedTime = 0.f;
}

bool UGamePlayerStatusComponent::GetExhausted() const
{
	return ExhaustedTime < EXHAUSTE_TIME;
}
