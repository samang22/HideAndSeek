// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacterStatusComponent.h"

ULobbyCharacterStatusComponent::ULobbyCharacterStatusComponent()
{
	SetOnAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_IDLE);
}

void ULobbyCharacterStatusComponent::SetOnAnimationStatus(uint8 InBit)
{
	LobbyCharacterAnimationStatus |= InBit;
}

void ULobbyCharacterStatusComponent::SetOffAnimationStatus(uint8 InBit)
{
	LobbyCharacterAnimationStatus &= ~(InBit);
}

bool ULobbyCharacterStatusComponent::GetAnimStatus(uint8 InBit) const
{
	return LobbyCharacterAnimationStatus & InBit;
}

void ULobbyCharacterStatusComponent::SetLobbyCharacterStatus(LOBBY_CHARACTER_STATUS eStatus)
{
	eLobbyCharacterStatus = eStatus;

	switch (eLobbyCharacterStatus)
	{
	case LOBBY_CHARACTER_STATUS::IDLE:
		SetOnAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_IDLE);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_PICKED);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_PICKEDIDLE);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_UNPICKED);
		break;
	case LOBBY_CHARACTER_STATUS::PICKED:
		SetOnAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_PICKED);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_IDLE);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_PICKEDIDLE);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_UNPICKED);
		break;
	case LOBBY_CHARACTER_STATUS::PICKEDIDLE:
		SetOnAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_PICKEDIDLE);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_IDLE);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_PICKED);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_UNPICKED);		
		break;
	case LOBBY_CHARACTER_STATUS::UNPICKED:
		SetOnAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_UNPICKED);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_IDLE);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_PICKED);
		SetOffAnimationStatus(LOBBY_CHARACTER_ANIM_BIT_PICKEDIDLE);
		break;
	case LOBBY_CHARACTER_STATUS::END:
	default:
		_ASSERT(false);
		break;
	}


}
