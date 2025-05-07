// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/TriggerBox/EggCountTriggerBox.h"
#include "Actors/Game/NPC/Egg.h"

AEggCountTriggerBox::AEggCountTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &AEggCountTriggerBox::OnTriggerBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AEggCountTriggerBox::OnTriggerEndOverlap);
}

void AEggCountTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}

void AEggCountTriggerBox::OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (AEgg* Egg = Cast<AEgg>(OtherActor))
	{
		InsideEggs.Add(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("[EggCountTriggerBox] Entered: %s | Count: %d"), *OtherActor->GetName(), InsideEggs.Num());
	}
}

void AEggCountTriggerBox::OnTriggerEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (AEgg* Egg = Cast<AEgg>(OtherActor))
	{
		InsideEggs.Remove(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("[EggCountTriggerBox] Exited: %s | Count: %d"), *OtherActor->GetName(), InsideEggs.Num());
	}
}

int32 AEggCountTriggerBox::GetEggCount() const
{
	return InsideEggs.Num();
}
