// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Egg.generated.h"

class UEggStatusComponent;
class UAIPerceptionStimuliSourceComponent;

class UBoxComponent;

UCLASS()
class EGGTHIEFSHOWDOWN_API AEgg : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEgg();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UEggStatusComponent* StatusComponent;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionComponent;

	
public:
	bool IsCoolTime();
	void SetCoolTimeZero();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimuliSource;

protected:
	bool bIsHold = false;
public:
	void SetIsHold(bool bFlag) { bIsHold = bFlag; }
	bool GetIsHold() const { return bIsHold; }

protected:
	bool IsOnGround(FHitResult& _HitResult) const;
	FVector Velocity = FVector::Zero();
};
