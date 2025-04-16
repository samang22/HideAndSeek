// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/NPC/Egg.h"
#include "Components/StatusComponent/Game/EggStatusComponent.h"
#include "Misc/Utils.h"

// Sets default values
AEgg::AEgg()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Obj/pc07it00_egg_egg__egg_m.pc07it00_egg_egg__egg_m'"));
	check(Asset.Object);
	StaticMeshComponent->SetStaticMesh(Asset.Object);
	StaticMeshComponent->SetRelativeScale3D(FVector(EGG_SCALE, EGG_SCALE, EGG_SCALE));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(CollisionProfileName::Egg);


}

// Called when the game starts or when spawned
void AEgg::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEgg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AEgg::IsCoolTime()
{
	return StatusComponent->IsCoolTime();
}

void AEgg::SetCoolTimeZero()
{
	StatusComponent->SetCoolTimeZero();
}

