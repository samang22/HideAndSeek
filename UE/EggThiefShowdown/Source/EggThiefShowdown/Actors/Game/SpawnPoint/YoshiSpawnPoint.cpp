// Fill out your copyright notice in the Description page of Project Settings.


#include "YoshiSpawnPoint.h"

// Sets default values
AYoshiSpawnPoint::AYoshiSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void AYoshiSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AYoshiSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

