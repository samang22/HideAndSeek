// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMario.h"
#include "../../Data/LobbyCharacterTableRow.h"
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

void ALobbyMario::PlayMontage(LOBBY_CHARACTER_MONTAGE _InEnum, bool bIsLoop)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	UAnimMontage* tempMontage = nullptr;

	switch (_InEnum)
	{
	case LOBBY_CHARACTER_MONTAGE::IDLE:
		tempMontage = LobbyCharacterData->IdleMontage;
		break;
	case LOBBY_CHARACTER_MONTAGE::PICKED:
		tempMontage = LobbyCharacterData->PickedMontage;
		break;
	case LOBBY_CHARACTER_MONTAGE::PICKEDIDLE:
		tempMontage = LobbyCharacterData->PickedIdleMontage;
		break;
	case LOBBY_CHARACTER_MONTAGE::UNPICKED:
		tempMontage = LobbyCharacterData->UnPickedMontage;
		break;
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

bool ALobbyMario::IsMontage(LOBBY_CHARACTER_MONTAGE _InEnum)
{
	if (!LobbyCharacterData) return false;

	switch (_InEnum)
	{
	case LOBBY_CHARACTER_MONTAGE::IDLE:
		return LobbyCharacterData->IdleMontage ? true : false;
	case LOBBY_CHARACTER_MONTAGE::PICKED:
		return LobbyCharacterData->PickedMontage ? true : false;
	case LOBBY_CHARACTER_MONTAGE::PICKEDIDLE:
		return LobbyCharacterData->PickedIdleMontage ? true : false;
	case LOBBY_CHARACTER_MONTAGE::UNPICKED:
		return LobbyCharacterData->UnPickedMontage ? true : false;
	default:
		return false;
	}
}

bool ALobbyMario::IsPlayingMontage(LOBBY_CHARACTER_MONTAGE _InEnum)
{
	if (!LobbyCharacterData) return false;
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	switch (_InEnum)
	{
	case LOBBY_CHARACTER_MONTAGE::IDLE:
		return AnimInstance->Montage_IsPlaying(LobbyCharacterData->IdleMontage);
	case LOBBY_CHARACTER_MONTAGE::PICKED:
		return AnimInstance->Montage_IsPlaying(LobbyCharacterData->PickedMontage);
	case LOBBY_CHARACTER_MONTAGE::PICKEDIDLE:
		return AnimInstance->Montage_IsPlaying(LobbyCharacterData->PickedIdleMontage);
	case LOBBY_CHARACTER_MONTAGE::UNPICKED:
		return AnimInstance->Montage_IsPlaying(LobbyCharacterData->UnPickedMontage);
	default:
		return false;
	}
}

