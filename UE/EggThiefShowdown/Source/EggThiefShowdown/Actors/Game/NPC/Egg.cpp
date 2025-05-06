// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/NPC/Egg.h"
#include "Components/StatusComponent/Game/EggStatusComponent.h"
#include "Misc/Utils.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Actors/Game/Character/GamePlayer.h"
#include "Actors/Game/Projectile/Projectile.h"
#include "Components/BoxComponent.h"

// Sets default values
AEgg::AEgg()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetBoxExtent(FVector(EGG_BOX_EXTENT, EGG_BOX_EXTENT, EGG_BOX_EXTENT));
	CollisionComponent->SetCollisionProfileName(CollisionProfileName::Egg);
	RootComponent = CollisionComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Obj/pc07it00_egg_egg__egg_m.pc07it00_egg_egg__egg_m'"));
	check(Asset.Object);
	StaticMeshComponent->SetStaticMesh(Asset.Object);
	StaticMeshComponent->SetRelativeScale3D(FVector(EGG_SCALE, EGG_SCALE, EGG_SCALE));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Stimuli Source 컴포넌트 생성
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	StatusComponent = CreateDefaultSubobject<UEggStatusComponent>(TEXT("StatusComponent"));

    bReplicates = true;
    SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AEgg::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->SetBoxExtent(FVector(EGG_BOX_EXTENT, EGG_BOX_EXTENT, EGG_BOX_EXTENT));

	// 퍼셉션 시스템에 등록
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server: AEgg %s BeginPlay. Registering with Perception System."), *GetName());
		// 시야 센스로 등록
		StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>(UAISense_Sight::StaticClass()));
		StimuliSource->RegisterWithPerceptionSystem();
	}
}

float AEgg::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageResult = StatusComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (AProjectile* Proj = Cast<AProjectile>(DamageCauser))
	{
		if (AGamePlayer* GP = Cast<AGamePlayer>(Proj->GetOwner()))
		{
			GP->SetEgg(this);
			SetIsHold(true);
		}
	}

	return DamageResult;
}

// Called every frame
void AEgg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (!GetIsHold())
		{
			FHitResult HitResult;
			if (IsOnGround(HitResult))
			{      
				Velocity = FVector::Zero();

					// 위치 보정
				SetActorLocation(HitResult.ImpactPoint + FVector(0, 0, EGG_BOX_EXTENT * 20)); // 필요 시
			}
			else
			{
				FVector Location = GetActorLocation();
				Velocity.Z -= 980.f * DeltaTime; // 수동 중력
				Location += Velocity * DeltaTime;
				SetActorLocation(Location);
			}
		}
	}
}

bool AEgg::IsCoolTime()
{
	return StatusComponent->IsCoolTime();
}

void AEgg::SetCoolTimeZero()
{
	StatusComponent->SetCoolTimeZero();
}

bool AEgg::IsOnGround(FHitResult& _HitResult) const
{
	FVector Start = GetActorLocation();  
	FVector End = Start - FVector(0, 0, 50.0f); // 바로 아래로 짧게 트레이스

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 자기 자신은 무시

	return GetWorld()->LineTraceSingleByChannel(_HitResult, Start, End, ECC_Visibility, Params);
}