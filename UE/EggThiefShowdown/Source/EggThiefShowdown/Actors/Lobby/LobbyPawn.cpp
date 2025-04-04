// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "../../Misc/Utils.h"

#define LOBBY_CAMERA_ORTHO_WIDTH 500.f

// Sets default values
ALobbyPawn::ALobbyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
}

// Called when the game starts or when spawned
void ALobbyPawn::BeginPlay()
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
void ALobbyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALobbyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

