// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer.h"
#include "../../../Components/StatusComponent/Game/GamePlayerStatusComponent.h"
#include "../../../Data/GamePlayerTableRow.h"
#include "Kismet/GameplayStatics.h"
#include "../../Lobby/LobbyCamera.h"



// Sets default values
AGamePlayer::AGamePlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkeletalMeshComponent;
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SkeletalMeshComponent->SetRelativeScale3D(FVector(CHARACTER_DEFAULT_SCALE, CHARACTER_DEFAULT_SCALE, CHARACTER_DEFAULT_SCALE));

	StatusComponent = CreateDefaultSubobject<UGamePlayerStatusComponent>(TEXT("StatusComponent"));
}

void AGamePlayer::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FGamePlayerTableRow* Data = DataTableRowHandle.GetRow<FGamePlayerTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	GamePlayerData = Data;

	SkeletalMeshComponent->SetSkeletalMesh(GamePlayerData->SkeletalMesh);
	SkeletalMeshComponent->SetAnimClass(GamePlayerData->AnimClass);
	SkeletalMeshComponent->SetRelativeScale3D(GamePlayerData->MeshTransform.GetScale3D());
	//SkeletalMeshComponent->SetRelativeTransform(LobbyCharacterData->MeshTransform);

	//StatusComponent->SetLobbyCharacterKind(LobbyCharacterData->eLobbyCharacter);
}

// Called when the game starts or when spawned
void AGamePlayer::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::BeginPlay"));

	Super::BeginPlay();
	
	SetData(DataTableRowHandle);
}

void AGamePlayer::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void AGamePlayer::PostLoad()
{
	Super::PostLoad();
}

void AGamePlayer::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void AGamePlayer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
	SetActorTransform(Transform);
}

void AGamePlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void AGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGamePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGamePlayer::PlayMontage(GAME_PLAYER_MONTAGE _InEnum, bool bIsLoop)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	UAnimMontage* tempMontage = nullptr;

	switch (_InEnum)
	{
	case GAME_PLAYER_MONTAGE::PICKUP:
		UE_LOG(LogTemp, Warning, TEXT("PlayMontage PICKUP"));
		tempMontage = GamePlayerData->PickupMontage;
		break;
	case GAME_PLAYER_MONTAGE::DAMAGED:
		UE_LOG(LogTemp, Warning, TEXT("PlayMontage DAMAGED"));
		tempMontage = GamePlayerData->DamagedMontage;
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

bool AGamePlayer::IsMontage(GAME_PLAYER_MONTAGE _InEnum)
{
	if (!GamePlayerData) return false;

	switch (_InEnum)
	{
	case GAME_PLAYER_MONTAGE::DAMAGED:
		return GamePlayerData->PickupMontage ? true : false;
	case GAME_PLAYER_MONTAGE::PICKUP:
		return GamePlayerData->DamagedMontage ? true : false;
	default:
		return false;
	}
}

bool AGamePlayer::IsPlayingMontage(GAME_PLAYER_MONTAGE _InEnum)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	switch (_InEnum)
	{
	case GAME_PLAYER_MONTAGE::DAMAGED:
		return AnimInstance->Montage_IsPlaying(GamePlayerData->PickupMontage);
	case GAME_PLAYER_MONTAGE::PICKUP:
		return AnimInstance->Montage_IsPlaying(GamePlayerData->DamagedMontage);
	default:
		return false;
	}
}

