// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "EggCountTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API AEggCountTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	AEggCountTriggerBox();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Tracking")
	TSet<AActor*> InsideEggs;

	UFUNCTION()
	void OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnTriggerEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

public:
	// 현재 트리거 안에 있는 에그 수를 반환
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	int32 GetEggCount() const;
};
