// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusComponent/Game/EggStatusComponent.h"

void UEggStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentCoolTime += DeltaTime;
}

UEggStatusComponent::UEggStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
