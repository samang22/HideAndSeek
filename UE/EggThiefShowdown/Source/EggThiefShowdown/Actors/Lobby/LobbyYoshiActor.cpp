// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Server/LobbyYoshiActor.h"


// Sets default values
ALobbyYoshiActor::ALobbyYoshiActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Players/Yoshi/pc07_yoshi_light.pc07_yoshi_light'"));
	check(Asset.Object);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetSkeletalMesh(Asset.Object);
	RootComponent = SkeletalMeshComponent;
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetRelativeScale3D(FVector(45.0, 45.0, 45.0));
}

void ALobbyYoshiActor::PostInitProperties()
{
	Super::PostInitProperties();
	SetRemoteRoleForBackwardsCompat(ROLE_AutonomousProxy);
}

// Called when the game starts or when spawned
void ALobbyYoshiActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyYoshiActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyYoshiActor::PlayMontage(LOBBY_CHARACTER_ENUM _InEnum, bool bIsLoop)
{
}

bool ALobbyYoshiActor::IsMontage(LOBBY_CHARACTER_ENUM _InEnum)
{
	return false;
}

bool ALobbyYoshiActor::IsPlayingMontage(LOBBY_CHARACTER_ENUM _InEnum)
{
	return false;
}

