// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"
#include "../../Data/LobbyCharacterTableRow.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../Components/StatusComponent/Lobby/LobbyCharacterStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "../../UI/LobbySelectButtonWidget.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

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

   	SelectButtonWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SelectButtonWidgetComponent"));
	SelectButtonWidgetComponent->SetupAttachment(RootComponent);
	float RelativeScale = 1.f / 90.f;
	SelectButtonWidgetComponent->SetRelativeScale3D(FVector(RelativeScale, RelativeScale, RelativeScale));
	SelectButtonWidgetComponent->SetRelativeLocation(FVector(1.f, RelativeScale * -0.5f, 0.f));
	static UClass* WidgetClass = LoadClass<ULobbySelectButtonWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/InGame/UI_YoshiSelectButton.UI_YoshiSelectButton_C'"));


	if (WidgetClass)
	{
		SelectButtonWidgetComponent->SetWidgetClass(WidgetClass);
		//SelectButtonWidgetComponent->SetDrawSize(FVector2D(30.f, 10.f)); // 위젯 크기 설정
		SelectButtonWidgetComponent->SetWidgetSpace(EWidgetSpace::World);

		//SelectButtonWidgetComponent->SetVisibility(true); // 위젯 보이기
	}

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
	
	UClass* WidgetClass = LoadClass<ULobbySelectButtonWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/InGame/UI_YoshiSelectButton.UI_YoshiSelectButton_C'"));
	check(WidgetClass);
	//ButtonWidget = CreateWidget<ULobbySelectButtonWidget>(GetWorld(), WidgetClass);

 	//SetData(DataTableRowHandle);

	if (SelectButtonWidgetComponent)
	{
		// 위젯을 마우스로 상호작용할 수 있도록 설정
		SelectButtonWidgetComponent->SetVisibility(true);
		SelectButtonWidgetComponent->Activate();

		// 마우스 포인터를 표시하고 클릭 가능하게 만들기
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeUIOnly());
		}
	}
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

	//if (SelectButtonWidgetComponent && GetWorld())
	//{
	//	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//	if (PlayerController)
	//	{
	//		FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	//		FVector WidgetLocation = SelectButtonWidgetComponent->GetComponentLocation();

	//		// 위젯이 카메라를 바라보게 회전
	//		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);
	//		SelectButtonWidgetComponent->SetWorldRotation(NewRotation);
	//	}
	//}

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

void ALobbyCharacter::SelectActor(const FString& InUserName)
{
	if (HasAuthority())  // 서버에서 처리
	{
		if (InUserName == TEXT(""))  // 아직 선택되지 않은 액터일 경우
		{
			UserName = InUserName;
			MulticastUpdateActorSelection(UserName);  // 선택 상태 동기화
		}
	}
	else
	{
		ServerSelectActor(UserName);  // 서버로 선택 요청
	}
}

void ALobbyCharacter::OnRep_SelectedPlayerID()
{
	// @TODO : Update UI 
}

void ALobbyCharacter::ServerSelectActor_Implementation(const FString& InUserName)
{
	if (UserName == TEXT(""))  // 선택되지 않았다면
	{
		UserName = InUserName;
		MulticastUpdateActorSelection(UserName);  // 모든 클라이언트에게 업데이트
	}
}

bool ALobbyCharacter::ServerSelectActor_Validate(const FString& InUserName)
{
	return true;
}

void ALobbyCharacter::MulticastUpdateActorSelection_Implementation(const FString& InUserName)
{
	// @TODO : 모든 클라이언트에서 선택 상태 업데이트

}

void ALobbyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyCharacter, UserName);
}
