// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/NPC/RealYoshiAIController.h"
#include "Actors/Game/NPC/Egg.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Components/SplineComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"



void ARealYoshiAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ARealYoshiAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ARealYoshiAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FindEggByPerception();
}

void ARealYoshiAIController::FindEggByPerception()
{
	APawn* OwningPawn = GetPawn();
	if (UAIPerceptionComponent* AIPerceptionComponent = OwningPawn->GetComponentByClass<UAIPerceptionComponent>())
	{
		TArray<AActor*> OutActors;
		AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), OutActors);

		bool bFound = false;
		for (AActor* It : OutActors)
		{
			if (AEgg* DetectedEgg = Cast<AEgg>(It))
			{
				bFound = true;
				Blackboard->SetValueAsObject(TEXT("DetectedEgg"), Cast<UObject>(DetectedEgg));
				break;
			}
		}
		if (!bFound)
		{
			Blackboard->ClearValue(TEXT("DetectedEgg"));
		}
	}
}
