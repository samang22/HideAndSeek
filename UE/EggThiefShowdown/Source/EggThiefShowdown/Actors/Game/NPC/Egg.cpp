// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/NPC/Egg.h"
#include "Components/StatusComponent/Game/EggStatusComponent.h"
#include "Misc/Utils.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

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

	// Stimuli Source 컴포넌트 생성
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	// 시야 센스로 등록
	StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>(UAISense_Sight::StaticClass()));

	StatusComponent = CreateDefaultSubobject<UEggStatusComponent>(TEXT("StatusComponent"));

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AEgg::BeginPlay()
{
	Super::BeginPlay();
	
	// 퍼셉션 시스템에 등록
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server: AEgg %s BeginPlay. Registering with Perception System."), *GetName());
		StimuliSource->RegisterWithPerceptionSystem();
	}
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

