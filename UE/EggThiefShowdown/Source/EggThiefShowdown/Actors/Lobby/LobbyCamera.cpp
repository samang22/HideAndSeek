// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "../../Misc/Utils.h"

#define LOBBY_CAMERA_ORTHO_WIDTH 500.f
// Sets default values
ALobbyCamera::ALobbyCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
}

// Called when the game starts or when spawned
void ALobbyCamera::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetViewTarget(this);
	}

	if (Camera)
	{
		Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
		Camera->OrthoWidth = LOBBY_CAMERA_ORTHO_WIDTH;  // 원하는 직교 투영의 크기로 설정
	}
}

// Called every frame
void ALobbyCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

