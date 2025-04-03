// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacterAnimInstance.h"
#include "../../Components/StatusComponent/Lobby/LobbyCharacterStatusComponent.h"
#include "../../Misc/Utils.h"
#include "../../Actors/Lobby/LobbyCharacter.h"

void ULobbyCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwnerActor = GetOwningActor();
	_ASSERT(OwnerActor);

	ALobbyCharacter* LobbyCharacter = Cast<ALobbyCharacter>(OwnerActor);
	if (LobbyCharacter)
	{
		StatusComponent = LobbyCharacter->GetStatusComponent();
	}
}

void ULobbyCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (StatusComponent)
	{
		bIsIdle = StatusComponent->GetAnimStatus(LOBBY_CHARACTER_ANIM_BIT_IDLE);
		bIsPickedIdle = StatusComponent->GetAnimStatus(LOBBY_CHARACTER_ANIM_BIT_PICKEDIDLE);
		//bIsPicked = StatusComponent->GetAnimStatus(LOBBY_CHARACTER_ANIM_BIT_PICKED);
		//bIsUnPicked = StatusComponent->GetAnimStatus(LOBBY_CHARACTER_ANIM_BIT_UNPICKED);
	}
}
