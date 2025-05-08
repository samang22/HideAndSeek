// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/TriggerBox/EggCountTriggerBox.h"
#include "Actors/Game/NPC/Egg.h"
#include "GameMode/GameMapGameMode.h"

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

		if (AGameMapGameMode* GameMode = Cast<AGameMapGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->UpdateEggCount();
			// 직접 값을 전달하지 않고, 간접적으로 갱신하는 이유는 맵에 여러 개 있는 트리거 박스의 개수를 다 갱신하기 위함임.
		}
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
