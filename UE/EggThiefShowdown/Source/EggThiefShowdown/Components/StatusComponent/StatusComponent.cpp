// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponent.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UStatusComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDie) { return 0.f; }
	if (FMath::IsNearlyZero(Damage)) { return 0.0; }
	float NewDamage = Damage;

	// Ex. NewDamage -= Armor;
	NewDamage = FMath::Clamp(NewDamage, 0.f, NewDamage);

	HP -= NewDamage;
	HP = FMath::Clamp(HP, 0.f, HP);

	LastInstigator = EventInstigator;
	//OnHPChanged.Broadcast(HP, MaxHP);

	if (HP == 0.f)
	{
		bDie = true;
		//OnDie.Broadcast();
	}

	return NewDamage;
}

