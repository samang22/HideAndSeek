// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"
#include "../../Data/LobbyCharacterTableRow.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../Components/StatusComponent/Lobby/LobbyCharacterStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "../../UI/LobbySelectCharacterButtonWidget.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystem/HaServerSubsystem.h"
#include "../../PlayerController/LobbyPlayerController.h"

// Sets default values
ALobbyCharacter::ALobbyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Players/Yoshi/pc07_yoshi_light.pc07_yoshi_light'"));
	//check(Asset.Object);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	//SkeletalMeshComponent->SetSkeletalMesh(Asset.Object);
	RootComponent = SkeletalMeshComponent;
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetRelativeScale3D(FVector(CHARACTER_DEFAULT_SCALE, CHARACTER_DEFAULT_SCALE, CHARACTER_DEFAULT_SCALE));


	StatusComponent = CreateDefaultSubobject<ULobbyCharacterStatusComponent>(TEXT("StatusComponent"));


   	SelectButtonWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SelectButtonWidgetComponent"));
	SelectButtonWidgetComponent->SetupAttachment(RootComponent);
	float RelativeScale = 1.f / CHARACTER_DEFAULT_SCALE;
	SelectButtonWidgetComponent->SetRelativeScale3D(FVector(RelativeScale, RelativeScale, RelativeScale));
	// Offset
	SelectButtonWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, -0.4f));
	static UClass* WidgetClass = LoadClass<ULobbySelectCharacterButtonWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/InGame/UI_LobbySelectCharacterButton.UI_LobbySelectCharacterButton_C'"));


	if (WidgetClass)
	{
		SelectButtonWidgetComponent->SetWidgetClass(WidgetClass);
		//SelectButtonWidgetComponent->SetDrawSize(FVector2D(30.f, 10.f)); // 위젯 크기 설정
		SelectButtonWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

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
	SkeletalMeshComponent->SetRelativeScale3D(LobbyCharacterData->MeshTransform.GetScale3D());
	//SkeletalMeshComponent->SetRelativeTransform(LobbyCharacterData->MeshTransform);
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

 	SetData(DataTableRowHandle);

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

		ULobbySelectCharacterButtonWidget* Widget = Cast<ULobbySelectCharacterButtonWidget>(SelectButtonWidgetComponent->GetWidget());
		if (Widget && LobbyCharacterData)
		{
			Widget->SetLobbyCharacterEnum(LobbyCharacterData->eLobbyCharacter);
			Widget->UpdateWidgetState();
		}

		if (Widget)
		{
			Widget->OnButtonEvent.AddDynamic(this, &ALobbyCharacter::SelectActor);
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

LOBBY_CHARACTER_SELECT_BUTTON_STATE ALobbyCharacter::GetSelectionState(const FString& InUserName)
{
	if (UserName.IsEmpty())
	{
		return LOBBY_CHARACTER_SELECT_BUTTON_STATE::EMPTY;
	}
	else if (UserName == InUserName)
	{
		return LOBBY_CHARACTER_SELECT_BUTTON_STATE::MY;
	}
	else
	{
		return LOBBY_CHARACTER_SELECT_BUTTON_STATE::OTHER;
	}
}

void ALobbyCharacter::PlayMontage(LOBBY_CHARACTER_MONTAGE _InEnum, bool bIsLoop)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayMontage"));
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	UAnimMontage* tempMontage = nullptr;

	switch (_InEnum)
	{
	case LOBBY_CHARACTER_MONTAGE::PICKED:
		UE_LOG(LogTemp, Warning, TEXT("PlayMontage PICKED"));
		tempMontage = LobbyCharacterData->PickedMontage;
		break;
	case LOBBY_CHARACTER_MONTAGE::UNPICKED:
		UE_LOG(LogTemp, Warning, TEXT("PlayMontage UNPICKED"));
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

void ALobbyCharacter::SetUserName(const FString& InUserName)
{
	UserName = InUserName;

	//MulticastUpdateActorSelection(InUserName);
}

void ALobbyCharacter::SelectActor(const FString& InUserName)
{
	if (HasAuthority())  // 서버에서 처리
	{
		UE_LOG(LogTemp, Warning, TEXT("서버에서 SelectActor 실행"));
	}
	else // 클라
	{
		UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 SelectActor 실행, 서버에 요청 중: %s"), *InUserName);

		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (!PC) return;

		ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC);
		if (LobbyPC)
		{
			LobbyPC->Server_SelectLobbyCharacter(this, InUserName);
			//ServerSelectActor(InUserName); 해당 함수를 PC에서 대신 호출해준다.
		}
	}

}

void ALobbyCharacter::OnRep_SelectedPlayerID()
{
	// @TODO : Update UI 
}

void ALobbyCharacter::ServerSelectActor_Implementation(const FString& InUserName)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerSelectActor_Implementation called by %s"), *InUserName);
	MulticastUpdateActorSelection(InUserName);
}

bool ALobbyCharacter::ServerSelectActor_Validate(const FString& InUserName)
{
	return true;
}

void ALobbyCharacter::MulticastUpdateActorSelection_Implementation(const FString& InUserName)
{
	UE_LOG(LogTemp, Warning, TEXT("MulticastUpdateActorSelection_Implementation called by %s"), *InUserName);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC);
	if (!LobbyPC) return;


	if (UserName.IsEmpty())
	{
		// 이미 선택한 캐릭터가 있음
		if (LobbyPC->GetSelectedLobbyCharacter())
		{
			UE_LOG(LogTemp, Warning, TEXT("이미 선택한 캐릭터가 있음 by %s"), *InUserName);
			return;
		}


		LobbyPC->SetSelectedLobbyCharacter(this);
		UserName = InUserName;


		PlayMontage(LOBBY_CHARACTER_MONTAGE::PICKED);
	}
	else if (UserName == InUserName)
	{
		UE_LOG(LogTemp, Warning, TEXT("LobbyPC->SetSelectedLobbyCharacter by %s"), *InUserName);

		LobbyPC->SetSelectedLobbyCharacter(nullptr);
		UserName = TEXT("");

		PlayMontage(LOBBY_CHARACTER_MONTAGE::UNPICKED);
	}
	else // UserName != InUserName
	{
		return;
	}

	// UI 처리
	if (SelectButtonWidgetComponent)
	{
		ULobbySelectCharacterButtonWidget* Widget = Cast<ULobbySelectCharacterButtonWidget>(SelectButtonWidgetComponent->GetWidget());
		if (Widget)
		{
			Widget->SetUserName(UserName);
			Widget->UpdateWidgetState(); // 예: 색상이나 텍스트 변경
		}
	}
}

void ALobbyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyCharacter, UserName);
}
