// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer.h"
#include "../../../Components/StatusComponent/Game/GamePlayerStatusComponent.h"
#include "../../../Data/GamePlayerTableRow.h"
#include "Kismet/GameplayStatics.h"
#include "../../Lobby/LobbyCamera.h"
#include "../SpawnPoint/MarioSpawnPoint.h"
#include "../SpawnPoint/YoshiSpawnPoint.h"
#include "../../../PlayerState/GameMapPlayerState.h"
#include "../../../PlayerState/LobbyMapPlayerState.h"
#include "../../../PlayerController/GameMapPlayerController.h"
#include "../../../PlayerController/LobbyPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	if (!Data) 
	{ 
		return; 
	}
	GamePlayerData = Data;

	SkeletalMeshComponent->SetSkeletalMesh(GamePlayerData->SkeletalMesh);
	SkeletalMeshComponent->SetAnimClass(GamePlayerData->AnimClass);
	SkeletalMeshComponent->SetRelativeScale3D(GamePlayerData->MeshTransform.GetScale3D());
	//SkeletalMeshComponent->SetRelativeTransform(LobbyCharacterData->MeshTransform);

	//StatusComponent->SetLobbyCharacterKind(LobbyCharacterData->eLobbyCharacter);
}

// Called when the game starts or when spawned
// 이 시점에 Controller가 Pawn을 Posess 했을 거라는 보장이 없음
void AGamePlayer::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::BeginPlay"));

	Super::BeginPlay();
	

}

void AGamePlayer::PossessedBy(AController* NewController)
{
	UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::PossessedBy"));

	Super::PossessedBy(NewController);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (ALobbyPlayerController* LPC = Cast<ALobbyPlayerController>(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::PossessedBy // ALobbyPlayerController"));

		if (ALobbyMapPlayerState* NewPlayerState = LPC->GetPlayerState<ALobbyMapPlayerState>())
		{
			UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::PossessedBy // ALobbyMapPlayerState"));


			switch (static_cast<LOBBY_CHARACTER_KIND>(NewPlayerState->GetLobbyCharacterKind()))
			{
			case LOBBY_CHARACTER_KIND::MARIO:
			{
				UE_LOG(LogTemp, Warning, TEXT("MARIO"));

				TArray<AActor*> SpawnPoints;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMarioSpawnPoint::StaticClass(), SpawnPoints);

				if (SpawnPoints.Num() > 0)
				{
					while (true)
					{
						int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
						AMarioSpawnPoint* MarioSP = Cast<AMarioSpawnPoint>(SpawnPoints[RandomIndex]);
						if (MarioSP->GetIsSpawned()) continue;

						MarioSP->SetIsSpawned(true);
						SetActorLocationAndRotation(MarioSP->GetActorLocation(), MarioSP->GetActorRotation().Quaternion());
						break;

					}

					static UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_GamePlayer.DT_GamePlayer"));

					if (DataTable)
					{
						FDataTableRowHandle MarioDataTableRowHandle;
						MarioDataTableRowHandle.DataTable = DataTable;
						MarioDataTableRowHandle.RowName = FName(TEXT("Mario"));
						DataTableRowHandle = MarioDataTableRowHandle;
					}
				}
			}

			break;
			case LOBBY_CHARACTER_KIND::YOSHI:
			{
				UE_LOG(LogTemp, Warning, TEXT("YOSHI"));

				TArray<AActor*> SpawnPoints;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AYoshiSpawnPoint::StaticClass(), SpawnPoints);

				if (SpawnPoints.Num() > 0)
				{
					while (true)
					{
						int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
						AYoshiSpawnPoint* YoshiSP = Cast<AYoshiSpawnPoint>(SpawnPoints[RandomIndex]);
						if (YoshiSP->GetIsSpawned()) continue;

						YoshiSP->SetIsSpawned(true);
						SetActorLocationAndRotation(YoshiSP->GetActorLocation(), YoshiSP->GetActorRotation().Quaternion());
						break;
					}
					
					static UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_GamePlayer.DT_GamePlayer"));

					if (DataTable)
					{
						FDataTableRowHandle YoshiDataTableRowHandle;
						YoshiDataTableRowHandle.DataTable = DataTable;
						YoshiDataTableRowHandle.RowName = FName(TEXT("Yoshi"));
						DataTableRowHandle = YoshiDataTableRowHandle;
					}
				}
			}
			break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("no MARIO no YOSHI"));
				return;
				break;
			}
		}
	}

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

	if (HasAuthority())
	{
		OnRep_UpdateDataTableRowHandle();
	}

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

void AGamePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ThisClass, DataTableRowHandle);
	DOREPLIFETIME(ThisClass, RowName);
}

void AGamePlayer::OnRep_UpdateDataTableRowHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::OnRep_UpdateDataTableRowHandle"));

	if (DataTableRowHandle.IsNull()) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::OnRep_UpdateDataTableRowHandle // DataTableRowHandle.IsNull()"));
		return; 
	}



	FGamePlayerTableRow* Data = DataTableRowHandle.GetRow<FGamePlayerTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }

	GamePlayerData = Data;
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		Movement->bOrientRotationToMovement = true;
		Movement->GetNavAgentPropertiesRef().bCanCrouch = true;
		// @TODO change it to data
		Movement->MaxWalkSpeed = 10.f;
		const float NewCapsuleHalfHeight = 45.f;
		//Movement->SetCrouchedHalfHeight(NewCapsuleHalfHeight);
	}

	{
		SkeletalMeshComponent = GetMesh();
		SkeletalMeshComponent->SetSkeletalMesh(GamePlayerData->SkeletalMesh);
		SkeletalMeshComponent->SetRelativeTransform(GamePlayerData->MeshTransform);
		SkeletalMeshComponent->SetAnimClass(GamePlayerData->AnimClass);

		// ACharacter::PostInitializeComponents() 시점에 초기에 설정된 Mesh의 RelativeLocation, RelativeRotation을 받아와서
		// CharacterMovementComponent에서 사용하고 있음.
		// 우리는 데이터를 받아와서 적용하고 있기 때문에 한번 더 설정 필요
		// * 이부분을 뺴면 내 Character는 정상적으로 보이지만, 다른 플레이어의 Character는 이동되기전 위치 정보로 처리됨
		// 이 데이터를 통해서 클라 UCharacterMovementComponent::SmoothClientPosition_UpdateVisuals() 에서 Mesh의 위치를 바꾸고 있음.
		// (Mesh->SetRelativeLocationAndRotation(NewRelTranslation, NewRelRotation, false, nullptr, GetTeleportType());)
		CacheInitialMeshOffset(GamePlayerData->MeshTransform.GetTranslation(), GamePlayerData->MeshTransform.GetRotation().Rotator());
	}
}

void AGamePlayer::OnRep_RowName()
{
	UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::OnRep_RowName"));

	// RowName을 기반으로 DataTableRowHandle 구성
	static UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_GamePlayer.DT_GamePlayer"));

	DataTableRowHandle.DataTable = DataTable;
	DataTableRowHandle.RowName = RowName;

	// 기존 OnRep_UpdateDataTableRowHandle 내용 호출
	OnRep_UpdateDataTableRowHandle();
}

