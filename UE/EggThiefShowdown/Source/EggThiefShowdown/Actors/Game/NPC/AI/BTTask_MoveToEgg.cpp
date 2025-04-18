// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/NPC/AI/BTTask_MoveToEgg.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Utils.h"
#include "Actors/Game/NPC/RealYoshi.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIAsyncTaskBlueprintProxy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Actors/Game/NPC/Egg.h"

UBTTask_MoveToEgg::UBTTask_MoveToEgg()
{
	NodeName = "MoveToEgg";
	bCreateNodeInstance = true;
	bTickIntervals = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToEgg::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIOwner = OwnerComp.GetAIOwner();

	APawn* Pawn = AIOwner->GetPawn();

	if (Pawn && Pawn->HasAuthority())
	{
		BehaviorTreeComponent = &OwnerComp;
		BlackboardComponent = OwnerComp.GetBlackboardComponent();

		ARealYoshi* RY = Cast<ARealYoshi>(AIOwner->GetPawn());
		if (!RY) return EBTNodeResult::Failed;
		AActor* SelectedEgg = Cast<AActor>(BlackboardComponent->GetValueAsObject(TEXT("SelectedEgg")));
		if (!SelectedEgg) return EBTNodeResult::Failed;

		FVector RYLocation = RY->GetActorLocation();
		FVector EggLocation = SelectedEgg->GetActorLocation();

		Proxy = UAIBlueprintHelperLibrary::CreateMoveToProxyObject(AIOwner, Pawn, EggLocation);
		if (Proxy)
		{
			Proxy->OnSuccess.AddDynamic(this, &ThisClass::OnResult);
			Proxy->OnFail.AddDynamic(this, &ThisClass::OnResult);

			return EBTNodeResult::InProgress;
		}
		else
		{
			return EBTNodeResult::Failed;
		}

	}

	return EBTNodeResult::Failed;
}

void UBTTask_MoveToEgg::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//APawn* Pawn = AIOwner->GetPawn();

	//UObject* DetectedEgg = BlackboardComponent->GetValueAsObject(TEXT("DetectedEgg"));
	//AEgg* Egg = Cast<AEgg>(DetectedEgg);
	//if (Pawn && Pawn->HasAuthority() && IsValid(DetectedEgg))
	//{
	//	FVector RYLocation = Pawn->GetActorLocation();
	//	FVector EggLocation = Egg->GetActorLocation();

	//	float Distance = FVector::Dist(RYLocation, EggLocation);
	//	if (Distance < 100.f)
	//	{
	//		AIOwner->StopMovement();
	//		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	}
	//}
}

void UBTTask_MoveToEgg::OnResult(EPathFollowingResult::Type MovementResult)
{
	FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Failed);
}
