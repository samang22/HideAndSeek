// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyYoshi.h"


// Sets default values
ALobbyYoshi::ALobbyYoshi()
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

void ALobbyYoshi::PostInitProperties()
{
	Super::PostInitProperties();
	SetRemoteRoleForBackwardsCompat(ROLE_AutonomousProxy);
}

// Called when the game starts or when spawned
void ALobbyYoshi::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyYoshi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyYoshi::PlayMontage(LOBBY_CHARACTER_ANIM_ENUM _InEnum, bool bIsLoop)
{
}

bool ALobbyYoshi::IsMontage(LOBBY_CHARACTER_ANIM_ENUM _InEnum)
{
	return false;
}

bool ALobbyYoshi::IsPlayingMontage(LOBBY_CHARACTER_ANIM_ENUM _InEnum)
{
	return false;
}

