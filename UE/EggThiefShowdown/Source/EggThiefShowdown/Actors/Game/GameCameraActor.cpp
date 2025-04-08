// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "../../Misc/Utils.h"

// Sets default values
AGameCameraActor::AGameCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
}

// Called when the game starts or when spawned
void AGameCameraActor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetViewTarget(this);
	}

	if (Camera)
	{
		Camera->ProjectionMode = ECameraProjectionMode::Perspective;;
	}
	
}

// Called every frame
void AGameCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

