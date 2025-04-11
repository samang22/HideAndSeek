// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "../Actors/Lobby/LobbyCharacter.h"
#include "../GameMode/LobbyMapGameMode.h"
#include "Subsystem/HaServerSubsystem.h"
#include "../PlayerState/LobbyMapPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/Lobby/LobbyCamera.h"
#include "../Actors/GameMapPlayerCameraManager.h"
#include "../Components/StatusComponent/StatusComponent.h"
#include "Kismet/KismetMathLibrary.h"


ALobbyPlayerController::ALobbyPlayerController()
{
    {
        static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
        { TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Data/Input/IMC_Character.IMC_Character'") };
        if (Asset.Object)
        {
            UE_LOG(LogTemp, Warning, TEXT("Set IMC_Character"));
            IMC_Default = Asset.Object;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No IMC_Character"));
        }
    }


    PlayerCameraManagerClass = AGameMapPlayerCameraManager::StaticClass();

}

void ALobbyPlayerController::Server_SelectLobbyCharacter_Implementation(ALobbyCharacter* LobbyCharacter, const FString& InUserName)
{
    if (LobbyCharacter)
    {
        LobbyCharacter->ServerSelectActor(InUserName);

        // 요청한 UserName과 LobbyCharacter의 UserName의 이름이 같다면, 이름이 바뀌었다는 것
        if (LobbyCharacter->GetUserName() == InUserName)
        {
            SetSelectedLobbyCharacter(LobbyCharacter);
        }
        else
        {
            SetSelectedLobbyCharacter(nullptr);
        }
    }

    ALobbyMapGameMode* GameMode = Cast<ALobbyMapGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->CheckAndServerTravel();
    }
}

void ALobbyPlayerController::SetSelectedLobbyCharacter(AActor* InCharacter)
{
    SelectedLobbyCharacter = InCharacter;
    if (SelectedLobbyCharacter)
    {
        // Local
        UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();
        _ASSERT(Subsystem);
        ALobbyCharacter* LC = Cast<ALobbyCharacter>(SelectedLobbyCharacter);
        LOBBY_CHARACTER_KIND eKind = LC->GetLobbyCharacterKind();
        Subsystem->SetLobbyCharacterKind(static_cast<uint8>(eKind));

        // Save it to next Level (Game Map)
        ALobbyMapPlayerState* LobbyMapPlayerState = Cast<ALobbyMapPlayerState>(PlayerState);
        LobbyMapPlayerState->SetLobbyCharacterKind(static_cast<uint8>(eKind));
    }
}

void ALobbyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    ensure(EnhancedInputComponent);

    if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Move")))
    {
        EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("IA_Move is disabled"));
    }

    if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_LookMouse")))
    {
        EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnLook);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("IA_LookMouse is disabled"));
    }
}

void ALobbyPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // OnPossess 서버에서만 실행
    StatusComponent = InPawn->GetComponentByClass<UStatusComponent>();
    UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::OnPossess"));

    //check(StatusComponent);
}

void ALobbyPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();
    UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::OnRep_Pawn()"));

    if (GetPawn())
    {
        StatusComponent = GetPawn()->GetComponentByClass<UStatusComponent>();
        //check(StatusComponent);
    }
    else
    {
        StatusComponent = nullptr;
    }
}

void ALobbyPlayerController::OnMove(const FInputActionValue& InputActionValue)
{
    if (StatusComponent && !StatusComponent->CanMove()) 
    {
        UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::OnMove Failed"));

        return; 
    }

    UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::OnMove Success"));


    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = K2_GetActorRotation();
    const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
    const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
    const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);

    APawn* ControlledPawn = GetPawn();
    ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);
    ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);

}

void ALobbyPlayerController::OnLook(const FInputActionValue& InputActionValue)
{
    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
    UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::OnLook"));

    AddYawInput(-1 * ActionValue.X);
    AddPitchInput(ActionValue.Y);
}

void ALobbyPlayerController::SetInputModeGameOnly()
{
    UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::SetInputModeGameOnly()"));

    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
    bShowMouseCursor = false; // UI도 같이 쓸 거면 true
}

void ALobbyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(IMC_Default, 0);
        if (IMC_Default)
        {
            UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::BeginPlay AddMappingContext Success"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::BeginPlay AddMappingContext Failed"));
        }
    }
}

