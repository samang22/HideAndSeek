// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "Actors/Lobby/LobbyCharacter.h"
#include "Actors/Lobby/LobbyCamera.h"
#include "Actors/GameMapPlayerCameraManager.h"
#include "Actors/Game/Character/GamePlayer.h"
#include "Actors/Game/Character/CustomSpectatorPawn.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "Components/StatusComponent/Game/GamePlayerStatusComponent.h"

#include "GameMode/LobbyMapGameMode.h"
#include "Subsystem/HaServerSubsystem.h"
#include "PlayerState/LobbyMapPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "../Plugins/Network/Source/Network/Public/UI/ChatWidget.h"

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

    SetReplicates(true);
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
        EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnMoveOff);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("IA_Move is disabled"));
    }

    if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Run")))
    {
        EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnRun);
        EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnRunOff);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("IA_Run is disabled"));
    }

    if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Attack")))
    {
        EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnAttack);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("IA_Attack is disabled"));
    }

    if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_LookMouse")))
    {
        EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnLook);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("IA_LookMouse is disabled"));
    }

    if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_DropEgg")))
    {
        EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnDropEgg);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("IA_DropEgg is disabled"));
    }

    if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_RightClick")))
    {
        EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnRightClick);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("IA_RightClick is disabled"));
    }

    if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Enter")))
    {
        EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnEnter);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("IA_Enter is disabled"));
    }
}

void ALobbyPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // OnPossess 서버에서만 실행
    StatusComponent = InPawn->GetComponentByClass<UGamePlayerStatusComponent>();
    UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::OnPossess"));

    //check(StatusComponent);
}

void ALobbyPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();
    UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::OnRep_Pawn()"));         

    if (GetPawn())
    {
        StatusComponent = GetPawn()->GetComponentByClass<UGamePlayerStatusComponent>();
        //check(StatusComponent);
    }
    else
    {
        StatusComponent = nullptr;
    }
}

void ALobbyPlayerController::OnMove(const FInputActionValue& InputActionValue)
{
    if (bStartChat) return;

    if (StatusComponent && !StatusComponent->CanMove())
    {
        UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::OnMove Failed"));
        return;
    }



    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = K2_GetActorRotation();
    const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
    const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
    const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);

    // 서버에서 계산
    if (ACustomSpectatorPawn* TempSpectatorPawn = Cast<ACustomSpectatorPawn>(GetPawn()))
    {
        TempSpectatorPawn->AddMovementInput(ForwardVector, ActionValue.X);
        TempSpectatorPawn->AddMovementInput(RightVector, ActionValue.Y);
	}
    else if (APawn* ControlledPawn = GetPawn())
    {
        ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);
        ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);
    }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::OnMove Failed - No Pawn"));

        if (APawn* CurrentPawn = GetPawn())
        {
            UE_LOG(LogTemp, Warning, TEXT("Current Pawn Class: %s"), *CurrentPawn->GetClass()->GetName());
        }
	}



    if (!HasAuthority()) // 클라이언트에서 로컬 움직임 시뮬레이션
    {
        Server_OnMove(InputActionValue); // 입력 정보를 서버로 전송
    }
    if (StatusComponent) StatusComponent->Server_SetOnAnimationStatus(GP_ANIM_BIT_WALK);
}

void ALobbyPlayerController::Server_OnMove_Implementation(const FInputActionValue& InputActionValue)
{
    if (StatusComponent && !StatusComponent->CanMove())
    {
        UE_LOG(LogTemp, Warning, TEXT("Server_OnMove Failed - Can't Move"));
        return;
    }

    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = K2_GetActorRotation();
    const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
    const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
    const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);

    if (ACustomSpectatorPawn* TempSpectatorPawn = Cast<ACustomSpectatorPawn>(GetPawn()))
    {
        TempSpectatorPawn->AddMovementInput(ForwardVector, ActionValue.X);
        TempSpectatorPawn->AddMovementInput(RightVector, ActionValue.Y);
	}
    else if (APawn* ControlledPawn = GetPawn())
    {
        ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);
        ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);
        StatusComponent->Server_SetOnAnimationStatus(GP_ANIM_BIT_WALK);
    }    

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server_OnMove Failed - No Pawn"));   

        if (APawn* CurrentPawn = GetPawn())
        {
            UE_LOG(LogTemp, Warning, TEXT("Current Pawn Class: %s"), *CurrentPawn->GetClass()->GetName());
        }
    }
}

void ALobbyPlayerController::OnMoveOff(const FInputActionValue& InputActionValue)
{
    if (StatusComponent) StatusComponent->Server_SetOffAnimationStatus(GP_ANIM_BIT_WALK);
}

void ALobbyPlayerController::OnLook(const FInputActionValue& InputActionValue)
{
    if (bStartChat) return;

    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

    AddYawInput(-1 * ActionValue.X);
    AddPitchInput(ActionValue.Y);
}

void ALobbyPlayerController::OnRun(const FInputActionValue& InputActionValue)
{
    if (bStartChat) return;

    if (StatusComponent && StatusComponent->GetExhausted()) return;

    APawn* ControlledPawn = GetPawn();
    if (AGamePlayer* GP = Cast<AGamePlayer>(ControlledPawn))
    {
        if (!GP->GetIsRun() && !GP->GetIsEgg())
        {
            if (GP->GetStatusComponent()->CanSprint() && !GP->GetStatusComponent()->GetExhausted())
            {
                GP->Server_SetSpeedRun();
                if (StatusComponent) StatusComponent->Server_SetOnAnimationStatus(GP_ANIM_BIT_RUN);
            }
        }
    }
}

void ALobbyPlayerController::OnRunOff(const FInputActionValue& InputActionValue)
{
    if (StatusComponent) StatusComponent->Server_SetOffAnimationStatus(GP_ANIM_BIT_RUN);

    APawn* ControlledPawn = GetPawn();
    if (AGamePlayer* GP = Cast<AGamePlayer>(ControlledPawn))
    {
        GP->Server_SetSpeedWalk();
    }
}

void ALobbyPlayerController::OnAttack(const FInputActionValue& InputActionValue)
{
    if (bStartChat) return;

    if (AGamePlayer* GP = Cast<AGamePlayer>(GetPawn()))
    {
        if (!GP->GetIsEgg())
        {
            if (GP->IsMontage(GAME_PLAYER_MONTAGE::PICKUP))
            {
                GP->Server_PlayMontage((uint8)GAME_PLAYER_MONTAGE::PICKUP);
            }
        }
    }
}

void ALobbyPlayerController::OnDropEgg(const FInputActionValue& InputActionValue)
{
    if (bStartChat) return;

    if (AGamePlayer* GP = Cast<AGamePlayer>(GetPawn()))
    {
        if (GP->GetIsEgg())
        { 
            if (GP->IsMontage(GAME_PLAYER_MONTAGE::DROPEGG))
            {
                GP->Server_PlayMontage((uint8)GAME_PLAYER_MONTAGE::DROPEGG);
            }
        }
    }
}

void ALobbyPlayerController::OnRightClick(const FInputActionValue& InputActionValue)
{
    if (bStartChat) return;

    if (AGamePlayer* GP = Cast<AGamePlayer>(GetPawn()))
    {
        if (!GP->GetIsEgg())
        {
            if (GP->IsMontage(GAME_PLAYER_MONTAGE::ATTACK))
            {
                GP->Server_PlayMontage((uint8)GAME_PLAYER_MONTAGE::ATTACK);
            }
        }
    }
}

void ALobbyPlayerController::OnEnter(const FInputActionValue& InputActionValue)
{
    if (AGamePlayer* GP = Cast<AGamePlayer>(GetPawn()))
    {
        if (UChatWidget* ChatWidget = GP->GetChatWidget())
        {
            SetInputModeGameOnly(bStartChat);
            bStartChat = !bStartChat;
        }
    }
}

void ALobbyPlayerController::SetInputModeGameOnly(bool bFlag)
{
    UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::SetInputModeGameOnly()"));

    if (bFlag)
    {
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
        bShowMouseCursor = false; // UI도 같이 쓸 거면 true
    }
    else
    {
        FInputModeGameAndUI InputMode;
        SetInputMode(InputMode);
        bShowMouseCursor = true; // UI도 같이 쓸 거면 true
    }
}

void ALobbyPlayerController::Client_ExposeGameResultWidget_Implementation(bool bFlag)
{
    if (AGamePlayer* GP = Cast<AGamePlayer>(GetPawn()))
    {
        GP->ExposeGameResultWidget(bFlag);
    }
}

void ALobbyPlayerController::Client_ExposeTimeLimitWidget_Implementation(bool bFlag)
{
    if (AGamePlayer* GP = Cast<AGamePlayer>(GetPawn()))
    {
        GP->ExposeTimeLimitWidget(bFlag);
	}
}

void ALobbyPlayerController::Client_UpdateTimeLimit_Implementation(int32 MinuteTen, int32 MinuteOne, int32 SecondTen, int32 SecondOne)
{
    if (AGamePlayer* GP = Cast<AGamePlayer>(GetPawn()))
    {
        GP->UpdateTimeLimitWidget(MinuteTen, MinuteOne, SecondTen, SecondOne);
    }
}

void ALobbyPlayerController::Client_UpdateGameEnd_Implementation(bool bFlag)
{
    UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::Client_UpdateGameEnd_Implementation"));
    if (AGamePlayer* GP = Cast<AGamePlayer>(GetPawn()))
    {
        GP->UpdateGameResultWidget(bFlag);
    }
}

void ALobbyPlayerController::Client_UpdateCountdown_Implementation(int32 Countdown)
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::Client_UpdateCountdown_Implementation"));
    if (AGamePlayer* GP = Cast<AGamePlayer>(GetPawn()))
    {
		GP->SetCountdown(Countdown);    
    }
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

