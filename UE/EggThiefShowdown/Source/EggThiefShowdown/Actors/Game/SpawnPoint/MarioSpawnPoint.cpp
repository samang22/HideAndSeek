// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioSpawnPoint.h"

// Sets default values
AMarioSpawnPoint::AMarioSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void AMarioSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMarioSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

