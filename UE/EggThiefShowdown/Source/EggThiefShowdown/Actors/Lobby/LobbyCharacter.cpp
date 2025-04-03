// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"
#include "../../Data/LobbyCharacterTableRow.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../Components/StatusComponent/Lobby/LobbyCharacterStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ALobbyCharacter::ALobbyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Players/Yoshi/pc07_yoshi_light.pc07_yoshi_light'"));
	//check(Asset.Object);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	//SkeletalMeshComponent->SetSkeletalMesh(Asset.Object);
	RootComponent = SkeletalMeshComponent;
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetRelativeScale3D(FVector(45.0, 45.0, 45.0));

	StatusComponent = CreateDefaultSubobject<ULobbyCharacterStatusComponent>(TEXT("StatusComponent"));

	//ButtonWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	//ButtonWidget->SetupAttachment(RootComponent);
}

void ALobbyCharacter::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FLobbyCharacterTableRow* Data = DataTableRowHandle.GetRow<FLobbyCharacterTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	LobbyCharacterData = Data;

	SkeletalMeshComponent->SetSkeletalMesh(LobbyCharacterData->SkeletalMesh);
	SkeletalMeshComponent->SetAnimClass(LobbyCharacterData->AnimClass);
	SkeletalMeshComponent->SetRelativeTransform(LobbyCharacterData->MeshTransform);
}

void ALobbyCharacter::PostInitProperties()
{
	Super::PostInitProperties();
	SetRemoteRoleForBackwardsCompat(ROLE_AutonomousProxy);
}

// Called when the game starts or when spawned
void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	//SetData(DataTableRowHandle);
}

void ALobbyCharacter::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void ALobbyCharacter::PostLoad()
{
	Super::PostLoad();
}

void ALobbyCharacter::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void ALobbyCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
	SetActorTransform(Transform);
}

void ALobbyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const LOBBY_CHARACTER_STATUS eStatus = StatusComponent->GetLobbyCharacterStatus();

}

void ALobbyCharacter::PlayMontage(LOBBY_CHARACTER_MONTAGE _InEnum, bool bIsLoop)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	UAnimMontage* tempMontage = nullptr;

	switch (_InEnum)
	{
	case LOBBY_CHARACTER_MONTAGE::PICKED:
		tempMontage = LobbyCharacterData->PickedMontage;
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

bool ALobbyCharacter::IsMontage(LOBBY_CHARACTER_MONTAGE _InEnum)
{
	if (!LobbyCharacterData) return false;

	switch (_InEnum)
	{
	case LOBBY_CHARACTER_MONTAGE::PICKED:
		return LobbyCharacterData->PickedMontage ? true : false;
	case LOBBY_CHARACTER_MONTAGE::UNPICKED:
		return LobbyCharacterData->UnPickedMontage ? true : false;
	default:
		return false;
	}
}

bool ALobbyCharacter::IsPlayingMontage(LOBBY_CHARACTER_MONTAGE _InEnum)
{
	if (!LobbyCharacterData) return false;
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	switch (_InEnum)
	{
	case LOBBY_CHARACTER_MONTAGE::PICKED:
		return AnimInstance->Montage_IsPlaying(LobbyCharacterData->PickedMontage);
	case LOBBY_CHARACTER_MONTAGE::UNPICKED:
		return AnimInstance->Montage_IsPlaying(LobbyCharacterData->UnPickedMontage);
	default:
		return false;
	}
}

