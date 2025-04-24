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
#include "../../../PlayerController/LobbyPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../Components/SoftWheelSpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"

#include "Actors/Game/NPC/Egg.h"

// Sets default values
AGamePlayer::AGamePlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);
	bAlwaysRelevant = true;

	UCapsuleComponent* tempCapsuleComponent = GetCapsuleComponent();
	tempCapsuleComponent->SetCanEverAffectNavigation(false);
	RootComponent = tempCapsuleComponent;
	tempCapsuleComponent->SetCollisionProfileName(CollisionProfileName::Yoshi);
	tempCapsuleComponent->SetCapsuleHalfHeight(CHARACTER_CAPSULE_HALF_HEIGHT);


	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SkeletalMeshComponent->SetRelativeScale3D(FVector(CHARACTER_DEFAULT_SCALE, CHARACTER_DEFAULT_SCALE, CHARACTER_DEFAULT_SCALE));
	SkeletalMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -1.f * CHARACTER_CAPSULE_HALF_HEIGHT));
	SkeletalMeshComponent->SetupAttachment(RootComponent);


	StatusComponent = CreateDefaultSubobject<UGamePlayerStatusComponent>(TEXT("StatusComponent"));
	StatusComponent->SetIsReplicated(true);


	SpringArm = CreateDefaultSubobject<USoftWheelSpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = CHARACTER_TARGETARMLENGTH;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	SpringArm->ProbeSize = PROBE_SIZE;

	SpringArm->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f)); // 약간 아래로 바라보게

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector::ZeroVector);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->SetIsReplicated(true);

	MovementComponent->NetworkSmoothingMode = ENetworkSmoothingMode::Linear;
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

	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
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

	if (HasAuthority())
	{
		InitDataTableByPlayerState();
		OnRep_UpdateDataTableRowHandle();         
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

void AGamePlayer::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (IsLocallyControlled())
	{
		ULevel* Level = GetLevel();
		if (Level)
		{
			FString LevelName = Level->GetOuter()->GetName();
			if (LevelName == TEXT("GameMap"))
			{
				ALobbyPlayerController* LPC = Cast<ALobbyPlayerController>(GetController());
				if (LPC)
				{
					FInputModeGameOnly InputMode;
					LPC->SetInputMode(InputMode);
					LPC->bShowMouseCursor = false;

				}

				InitDataTableByPlayerState();

			}
			else
			{

			}
		}

	}
}

void AGamePlayer::InitDataTableByPlayerState()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (ALobbyPlayerController* LPC = Cast<ALobbyPlayerController>(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::InitByDataTable // ALobbyPlayerController"));

		if (ALobbyMapPlayerState* NewPlayerState = LPC->GetPlayerState<ALobbyMapPlayerState>())
		{
			UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::InitByDataTable // ALobbyMapPlayerState"));


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

						//MarioSP->SetIsSpawned(true);
						FVector Location = MarioSP->GetActorLocation();
						SetActorLocationAndRotation(MarioSP->GetActorLocation(), MarioSP->GetActorRotation().Quaternion());
						UE_LOG(LogTemp, Warning, TEXT("MarioSP %f %f %f"), Location.X, Location.Y, Location.Z);

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

				StatusComponent->SetCharacterKind(LOBBY_CHARACTER_KIND::MARIO);
				UCapsuleComponent* tempCapsuleComponent = GetCapsuleComponent();
				tempCapsuleComponent->SetCollisionProfileName(CollisionProfileName::Mario);
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
						FVector Location = YoshiSP->GetActorLocation();
						SetActorLocationAndRotation(Location, YoshiSP->GetActorRotation().Quaternion());
						UE_LOG(LogTemp, Warning, TEXT("YoshiSP %f %f %f"), Location.X, Location.Y, Location.Z);

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

				StatusComponent->SetCharacterKind(LOBBY_CHARACTER_KIND::YOSHI);
				UCapsuleComponent* tempCapsuleComponent = GetCapsuleComponent();
				tempCapsuleComponent->SetCollisionProfileName(CollisionProfileName::Yoshi);
			}
			break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("no MARIO no YOSHI"));
				return;
				break;
			}
		}
	}
}

void AGamePlayer::SetInputModeGameOnly()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (ALobbyPlayerController* LPC = Cast<ALobbyPlayerController>(PC))
	{
		LPC->SetInputModeGameOnly();
		UE_LOG(LogTemp, Warning, TEXT("AGamePlayer::SetInputModeGameOnly Successed"));
	}
}

void AGamePlayer::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (!GamePlayerData)
	{
		Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
		return;
	}
	RecalculateBaseEyeHeight();
	FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
	MeshRelativeLocation.Z = GamePlayerData->MeshTransform.GetLocation().Z + HalfHeightAdjust;
	BaseTranslationOffset.Z = MeshRelativeLocation.Z;

	K2_OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AGamePlayer::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (!GamePlayerData)
	{
		Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
		return;
	}
	RecalculateBaseEyeHeight();

	FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
	MeshRelativeLocation.Z = GamePlayerData->MeshTransform.GetLocation().Z;
	BaseTranslationOffset.Z = MeshRelativeLocation.Z;

	K2_OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

// Called every frame
void AGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		SetEggRelativePosition(DeltaTime); // 그냥 바로 실행
	}
}

// Called to bind functionality to input
void AGamePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGamePlayer::PlayMontage(GAME_PLAYER_MONTAGE _InEnum, bool bIsLoop)
{
	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
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
	case GAME_PLAYER_MONTAGE::ATTACK:
		UE_LOG(LogTemp, Warning, TEXT("PlayMontage ATTACK"));
		tempMontage = GamePlayerData->AttackMontage;
		break;
	case GAME_PLAYER_MONTAGE::DROPEGG:
		UE_LOG(LogTemp, Warning, TEXT("PlayMontage DROPEGG"));
		tempMontage = GamePlayerData->DropEggMontage;
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

void AGamePlayer::Server_PlayMontage_Implementation(uint8 _InEnum, bool bIsLoop)
{
	Multicast_PlayMontage(_InEnum, bIsLoop);
}


void AGamePlayer::Multicast_PlayMontage_Implementation(uint8 _InEnum, bool bIsLoop)
{
	PlayMontage((GAME_PLAYER_MONTAGE)_InEnum, bIsLoop);
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
	case GAME_PLAYER_MONTAGE::ATTACK:
		return GamePlayerData->AttackMontage ? true : false;
	case GAME_PLAYER_MONTAGE::DROPEGG:
		return GamePlayerData->DropEggMontage ? true : false;
	default:
		return false;
	}
}

bool AGamePlayer::IsPlayingMontage(GAME_PLAYER_MONTAGE _InEnum)
{
	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();

	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	switch (_InEnum)
	{
	case GAME_PLAYER_MONTAGE::DAMAGED:
		return AnimInstance->Montage_IsPlaying(GamePlayerData->PickupMontage);
	case GAME_PLAYER_MONTAGE::PICKUP:
		return AnimInstance->Montage_IsPlaying(GamePlayerData->DamagedMontage);
	case GAME_PLAYER_MONTAGE::ATTACK:
		return AnimInstance->Montage_IsPlaying(GamePlayerData->AttackMontage);
	case GAME_PLAYER_MONTAGE::DROPEGG:
		return AnimInstance->Montage_IsPlaying(GamePlayerData->DropEggMontage);
	default:
		return false;
	}
}

void AGamePlayer::SetSpeedWalk()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();

	float TargetSpeed = 0.f;
	switch (StatusComponent->GetCharacterKind())
	{
	case LOBBY_CHARACTER_KIND::MARIO:
		TargetSpeed = MARIO_WALK_SPEED;
		break;
	case LOBBY_CHARACTER_KIND::YOSHI:
		TargetSpeed = YOSHI_WALK_SPEED;
		break;
	}

	if (!FMath::IsNearlyEqual(ReplicatedMaxWalkSpeed, TargetSpeed))
	{
		ReplicatedMaxWalkSpeed = TargetSpeed;
		OnRep_MaxWalkSpeed(); // 서버에서도 직접 적용
	}

	bIsRun = false;
}

void AGamePlayer::SetSpeedRun()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();

	float TargetSpeed = 0.f;
	switch (StatusComponent->GetCharacterKind())
	{
	case LOBBY_CHARACTER_KIND::MARIO:
		TargetSpeed = MARIO_RUN_SPEED;
		break;
	case LOBBY_CHARACTER_KIND::YOSHI:
		TargetSpeed = YOSHI_RUN_SPEED;
		break;
	}

	if (!FMath::IsNearlyEqual(ReplicatedMaxWalkSpeed, TargetSpeed))
	{
		ReplicatedMaxWalkSpeed = TargetSpeed;
		OnRep_MaxWalkSpeed(); // 서버에서도 직접 적용
	}

	bIsRun = true;
}

void AGamePlayer::OnRep_MaxWalkSpeed()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = ReplicatedMaxWalkSpeed;
	}
}

LOBBY_CHARACTER_KIND AGamePlayer::GetCharacterKind()
{
	return StatusComponent->GetCharacterKind();
}

float AGamePlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (StatusComponent->IsDie()) { return 0.f; }

	float DamageResult = StatusComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	// 데미지가 거의 들어오지 않음
	if (FMath::IsNearlyZero(DamageResult)) { return 0.f; }

	//// 만약 죽지 않았다면, DamagedMontage가 풀릴 때 다시 움직이게 만들기 
	//SetCanMove(false);

	// 계산 결과 사망
	if (StatusComponent->IsDie())
	{
		// @TODO : 사망처리
		SetActorEnableCollision(false);
		//PlayMontage(GAME_PLAYER_MONTAGE::DEAD);

	}
	// 데미지만 입음
	else if (!StatusComponent->IsDie() && GamePlayerData->DamagedMontage)
	{
		Server_PlayMontage((uint8)GAME_PLAYER_MONTAGE::DAMAGED);
	}

	// 알 처리 
	Server_DropEgg();



	return DamageResult;
}


void AGamePlayer::SetEggRelativePosition(float DeltaTime)
{
	if (Egg)
	{
		FVector GPLocation = GetActorLocation();
		FVector GPForward = GetActorForwardVector();
		FVector EggLocation = GPLocation + GPForward * YOSHI_EGG_HOLD_OFFSET;

		Egg->SetActorLocation(EggLocation);
	}
}

void AGamePlayer::Server_DropEgg_Implementation()
{
	Multicast_DropEgg();
}

void AGamePlayer::Multicast_DropEgg_Implementation()
{
	DropEgg();
}


void AGamePlayer::DropEgg()
{
	if (AEgg* CastedEgg = Cast<AEgg>(Egg))
	{
		CastedEgg->SetIsHold(false);
		Egg = nullptr;
	}
}

void AGamePlayer::SetCanMove(bool bFlag)
{
	StatusComponent->SetCanMove(bFlag);
}


void AGamePlayer::Server_SetSpeedWalk_Implementation()
{
	SetSpeedWalk();
}

void AGamePlayer::Server_SetSpeedRun_Implementation()
{
	SetSpeedRun();
}

void AGamePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);      

	DOREPLIFETIME(ThisClass, DataTableRowHandle);
	DOREPLIFETIME(ThisClass, ReplicatedMaxWalkSpeed);
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
		//Movement->bOrientRotationToMovement = true;
		//Movement->GetNavAgentPropertiesRef().bCanCrouch = true;
		//switch (StatusComponent->GetCharacterKind())
		//{
		//case LOBBY_CHARACTER_KIND::MARIO:
		//	if (StatusComponent->GetAnimStatus(GP_ANIM_BIT_RUN))
		//	{
		//		Movement->MaxWalkSpeed = MARIO_RUN_SPEED;
		//	}
		//	else
		//	{
		//		Movement->MaxWalkSpeed = MARIO_WALK_SPEED;
		//	}
		//	break;

		//case LOBBY_CHARACTER_KIND::YOSHI:
		//	if (StatusComponent->GetAnimStatus(GP_ANIM_BIT_RUN))
		//	{
		//		Movement->MaxWalkSpeed = YOSHI_RUN_SPEED;
		//	}
		//	else
		//	{
		//		Movement->MaxWalkSpeed = YOSHI_WALK_SPEED;
		//	}
		//	break;
		//}

		//const float NewCapsuleHalfHeight = CHARACTER_CAPSULE_HALF_HEIGHT;
		//Movement->SetCrouchedHalfHeight(NewCapsuleHalfHeight);
	}

	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		SkeletalMeshComponent->SetSkeletalMesh(GamePlayerData->SkeletalMesh);
		//SkeletalMeshComponent->SetRelativeTransform(GamePlayerData->MeshTransform);
		SkeletalMeshComponent->SetAnimClass(GamePlayerData->AnimClass);

		// ACharacter::PostInitializeComponents() 시점에 초기에 설정된 Mesh의 RelativeLocation, RelativeRotation을 받아와서
		// CharacterMovementComponent에서 사용하고 있음.
		// 데이터를 받아와서 적용하고 있기 때문에 한번 더 설정 필요
		// * 이부분을 뺴면 내 Character는 정상적으로 보이지만, 다른 플레이어의 Character는 이동되기전 위치 정보로 처리됨
		// 이 데이터를 통해서 클라 UCharacterMovementComponent::SmoothClientPosition_UpdateVisuals() 에서 Mesh의 위치를 바꾸고 있음.
		// (Mesh->SetRelativeLocationAndRotation(NewRelTranslation, NewRelRotation, false, nullptr, GetTeleportType());)
		//CacheInitialMeshOffset(GamePlayerData->MeshTransform.GetTranslation(), GamePlayerData->MeshTransform.GetRotation().Rotator());
	}
}