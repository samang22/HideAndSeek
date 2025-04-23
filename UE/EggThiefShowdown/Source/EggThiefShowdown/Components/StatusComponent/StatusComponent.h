// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float, CurrentHP, float, MaxHP);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EGGTHIEFSHOWDOWN_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusComponent();

	void SetAttack(const bool bFlag) { bAttack = bFlag; }
	virtual bool CanMove() const { return !bAttack && !bDie; }
	bool IsDie() const { return bDie; }
	float GetHP() const { return HP; }

	AController* GetLastInstigator() const { return LastInstigator; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

protected:
	bool bAttack = false;
	bool bDie = false;
	float MaxHP = 100.f;
	float HP = 100.f;

public:
	AController* LastInstigator = nullptr;

	//UPROPERTY(BlueprintAssignable)
	//FOnHPChanged OnHPChanged;
	//FOnDie OnDie;
};
