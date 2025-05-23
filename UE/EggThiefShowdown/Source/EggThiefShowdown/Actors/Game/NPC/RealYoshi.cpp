// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/NPC/RealYoshi.h"
#include "Actors/Game/NPC/RealYoshiAIController.h"
#include "Actors/Game/NPC/PatrolPath.h"
#include "Components/AdvancedFloatingPawnMovement.h"
#include "Components/StatusComponent/Game/RealYoshiStatusComponent.h"
#include "Data/GamePlayerTableRow.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/Utils.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARealYoshi::ARealYoshi(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(CollisionProfileName::Yoshi);
	CollisionComponent->SetCanEverAffectNavigation(false);

	MovementComponent = CreateDefaultSubobject<UAdvancedFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = YOSHI_WALK_SPEED;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -1.f * CHARACTER_CAPSULE_HALF_HEIGHT));

	AIControllerClass = ARealYoshiAIController::StaticClass();

	StatusComponent = CreateDefaultSubobject<URealYoshiStatusComponent>(TEXT("StatusComponent"));

}

void ARealYoshi::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FGamePlayerTableRow* Data = DataTableRowHandle.GetRow<FGamePlayerTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data)
	{
		return;
	}
	RealYoshiData = Data;

	SkeletalMeshComponent->SetSkeletalMesh(RealYoshiData->SkeletalMesh);
	SkeletalMeshComponent->SetAnimClass(RealYoshiData->AnimClass);
	SkeletalMeshComponent->SetRelativeScale3D(RealYoshiData->MeshTransform.GetScale3D());
	SkeletalMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -1 * RealYoshiData->CollisionSphereRadius));

	USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionComponent);
	SphereComponent->SetSphereRadius(RealYoshiData->CollisionSphereRadius);
	CollisionComponent->SetCollisionProfileName(CollisionProfileName::Yoshi);
	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
}

void ARealYoshi::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void ARealYoshi::PostLoad()
{
	Super::PostLoad();
}

void ARealYoshi::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void ARealYoshi::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

void ARealYoshi::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
	SetActorTransform(Transform);
}

// Called when the game starts or when spawned
void ARealYoshi::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
	SetData(DataTableRowHandle);

}

// 이 시점에서 Controller에 OwningPawn이 Set 되어있지 않다.
void ARealYoshi::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (PatrolPathRef)
	{
		if (ARealYoshiAIController* RealYoshiAIController = Cast<ARealYoshiAIController>(NewController))
		{
			RealYoshiAIController->SetPatrolPath(PatrolPathRef->GetPath(), this);
			RealYoshiAIController->SetPerceptionComponent(*AIPerceptionComponent);
		}
		else
		{
			check(false);
		}
	}
}


// Called every frame
void ARealYoshi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (bIsMoveToEgg)
		{
			MovementComponent->RequestDirectMove(MoveToEggDirection * MovementComponent->GetMaxSpeed(), true);
		}
		TickMovement(DeltaTime);
	}
}

float ARealYoshi::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageResult = StatusComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	// 만약 죽지 않았다면, DamagedMontage가 풀릴 때 다시 움직이게 만들기 
	StopMovement();

	//if (HasAuthority())
	//{
	//	PlayMontage(GAME_PLAYER_MONTAGE::DAMAGED);
	//}
	Server_PlayMontage((uint8)GAME_PLAYER_MONTAGE::DAMAGED);

	return DamageResult;
}

void ARealYoshi::OnRep_UpdateDataTableRowHandle()
{
}

void ARealYoshi::PlayMontage(GAME_PLAYER_MONTAGE _InEnum, bool bIsLoop)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	UAnimMontage* tempMontage = nullptr;

	switch (_InEnum)
	{
	case GAME_PLAYER_MONTAGE::PICKUP:
		UE_LOG(LogTemp, Warning, TEXT("PlayMontage PICKUP"));
		tempMontage = RealYoshiData->PickupMontage;
		break;
	case GAME_PLAYER_MONTAGE::DAMAGED:
		UE_LOG(LogTemp, Warning, TEXT("PlayMontage DAMAGED"));
		tempMontage = RealYoshiData->DamagedMontage;
		break;
		//case GAME_PLAYER_MONTAGE::ATTACK:
		//	UE_LOG(LogTemp, Warning, TEXT("PlayMontage ATTACK"));
		//	tempMontage = GamePlayerData->AttackMontage;
		//	break;
	default:
		break;
	}

	if (tempMontage/* && !AnimInstance->Montage_IsPlaying(tempMontage)*/)
	{
		if (bIsLoop)
		{
			AnimInstance->Montage_Play(tempMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		}
		else
		{
			AnimInstance->Montage_Play(tempMontage);
		}
	}
}

void ARealYoshi::Server_PlayMontage_Implementation(uint8 _InEnum, bool bIsLoop)
{
	Multicast_PlayMontage(_InEnum, bIsLoop);
}

void ARealYoshi::Multicast_PlayMontage_Implementation(uint8 _InEnum, bool bIsLoop)
{
	PlayMontage((GAME_PLAYER_MONTAGE)_InEnum, bIsLoop);
}

bool ARealYoshi::IsMontage(GAME_PLAYER_MONTAGE _InEnum)
{
	if (!RealYoshiData) return false;

	switch (_InEnum)
	{
	case GAME_PLAYER_MONTAGE::DAMAGED:
		return RealYoshiData->PickupMontage ? true : false;
	case GAME_PLAYER_MONTAGE::PICKUP:
		return false;
		//case GAME_PLAYER_MONTAGE::ATTACK:
		//	return GamePlayerData->AttackMontage ? true : false;
	default:
		return false;
	}
}

bool ARealYoshi::IsPlayingMontage(GAME_PLAYER_MONTAGE _InEnum)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	switch (_InEnum)
	{
	case GAME_PLAYER_MONTAGE::DAMAGED:
		return AnimInstance->Montage_IsPlaying(RealYoshiData->PickupMontage);
	case GAME_PLAYER_MONTAGE::PICKUP:
		return false;
		//case GAME_PLAYER_MONTAGE::ATTACK:
		//	return AnimInstance->Montage_IsPlaying(GamePlayerData->AttackMontage);
	default:
		return false;
	}
}

void ARealYoshi::TickMovement(float DeltaTime)
{
	if (!RealYoshiData) return;

	const float Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);

	if (!FMath::IsNearlyZero(Speed))
	{
		StatusComponent->Server_SetOnAnimationStatus(GP_ANIM_BIT_WALK);
	}
	else
	{
		StatusComponent->Server_SetOffAnimationStatus(GP_ANIM_BIT_WALK);
	}
}

void ARealYoshi::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, DataTableRowHandle);
}

void ARealYoshi::SetMoveToEggWithDirection(FVector _Direction)
{
	bIsMoveToEgg = true;
	MoveToEggDirection = _Direction;
}

void ARealYoshi::StopMovement()
{
	MovementComponent->MaxSpeed = 0.f;
}

void ARealYoshi::ResumeMovement()
{
	MovementComponent->MaxSpeed = YOSHI_WALK_SPEED;
}
