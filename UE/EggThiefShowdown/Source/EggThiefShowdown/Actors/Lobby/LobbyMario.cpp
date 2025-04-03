// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMario.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ALobbyMario::ALobbyMario()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Players/Mario/pc01_mario_light.pc01_mario_light'"));
	check(Asset.Object);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetSkeletalMesh(Asset.Object);
	RootComponent = SkeletalMeshComponent;
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetRelativeScale3D(FVector(45.0, 45.0, 45.0));
}

void ALobbyMario::PostInitProperties()
{
	Super::PostInitProperties();
	SetRemoteRoleForBackwardsCompat(ROLE_AutonomousProxy);
}

// Called when the game starts or when spawned
void ALobbyMario::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyMario::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyMario::PlayMontage(LOBBY_CHARACTER_ANIM_ENUM _InEnum, bool bIsLoop)
{
}

bool ALobbyMario::IsMontage(LOBBY_CHARACTER_ANIM_ENUM _InEnum)
{
	return false;
}

bool ALobbyMario::IsPlayingMontage(LOBBY_CHARACTER_ANIM_ENUM _InEnum)
{
	return false;
}

