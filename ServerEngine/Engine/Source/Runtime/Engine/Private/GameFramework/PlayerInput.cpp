#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"

TArray<FInputAxisKeyMapping> UPlayerInput::EngineDefinedAxisMappings;

UPlayerInput::UPlayerInput()
{
    if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject)) { return; }

    LastMouseState = InitialMouseState;
}

UPlayerInput::~UPlayerInput()
{
    if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject)) { return; }
    while (ShowCursor(TRUE) < 0);
}

void UPlayerInput::ProcessInputStack(const TArray<UInputComponent*>& InputComponentStack, const float DeltaTime, const bool bGamePaused)
{
    return;
    // @TODO
    APlayerController* PlayerController = GetOuterAPlayerController();
    if (PlayerController)
    {
        //PlayerController->PreProcessInput(DeltaTime, bGamePaused);
    }

    // EvaluateInputDelegates
    {
        struct FUpdateLastMouseState
        {
            ~FUpdateLastMouseState()
            {
                LastMouseState = CurrentMouseState;
            }
            DirectX::Mouse::State& LastMouseState;
            DirectX::Mouse::State& CurrentMouseState;
        };

        DirectX::Keyboard::State KeyboardState = DirectX::Keyboard::Get().GetState();

        DirectX::Mouse::State MouseState = DirectX::Mouse::Get().GetState();
        FUpdateLastMouseState UpdateLastMouseStateScope{ .LastMouseState = LastMouseState, .CurrentMouseState = MouseState };

        FVector2D Delta = FVector2D(MouseState.x - LastMouseState.x, LastMouseState.y - MouseState.y);


        for (UInputComponent* const InputComponent : InputComponentStack)
        {
            if (!InputComponent->AxisBindings.empty())
            {
                for (FInputAxisBinding& ItAxisBinding : InputComponent->AxisBindings)
                {
                    // 동일한 이름의 AxisKeyMapping 수집
                    TArray<FInputAxisKeyMapping> Results;
                    Results.reserve(EngineDefinedAxisMappings.size());
                    std::for_each(EngineDefinedAxisMappings.begin(), EngineDefinedAxisMappings.end(),
                        [&ItAxisBinding, &Results](FInputAxisKeyMapping& Mapping)
                        {
                            if (ItAxisBinding.AxisName == Mapping.AxisName)
                            {
                                Results.push_back(Mapping);
                            }
                        });

                    float Scale = 0.f;
                    for (auto& ItFind : Results)
                    {
                        if (ItFind.Key == EKeys::MouseX || ItFind.Key == EKeys::MouseY)
                        {
                            Scale = ItFind.Key == EKeys::MouseX ? Scale += Delta.x : Scale += Delta.y;
                            Scale *= ItFind.Scale;
                        }
                        else
                        {
                            const bool bKeyDown = KeyboardState.IsKeyDown(ItFind.Key);
                            if (bKeyDown)
                            {
                                Scale += ItFind.Scale;
                            }
                        }
                    }
                    ItAxisBinding.AxisDelegate.Broadcast(Scale);
                }
            }
        }
    }

    if (PlayerController)
    {
        //PlayerController->PostProcessInput(DeltaTime, bGamePaused);
    }
}

APlayerController* UPlayerInput::GetOuterAPlayerController() const
{
    return dynamic_cast<APlayerController*>(GetOuter());
}

void UPlayerInput::AddEngineDefinedAxisMapping(const FInputAxisKeyMapping& AxisMapping)
{
    auto It = find_if(EngineDefinedAxisMappings.begin(), EngineDefinedAxisMappings.end(),
        [&](const FInputAxisKeyMapping& Mapping)
        {
            return Mapping == AxisMapping;
        });
    if (It == EngineDefinedAxisMappings.end())
    {
        EngineDefinedAxisMappings.push_back(AxisMapping);
    }
    else
    {
        E_LOG(Error, TEXT("중복 FInputAxisKeyMapping 입니다."));
    }
}