// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/NPC/AI/BTTask_Base.h"
#include "AIController.h"

void UBTTask_Base::SetOwner(AActor* InActorOwner)
{
	ActorOwner = InActorOwner;
	AIOwner = Cast<AAIController>(InActorOwner);
}
