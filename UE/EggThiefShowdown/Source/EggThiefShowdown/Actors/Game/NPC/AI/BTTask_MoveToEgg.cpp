// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/NPC/AI/BTTask_MoveToEgg.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Utils.h"
#include "Actors/Game/NPC/RealYoshi.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToEgg::UBTTask_MoveToEgg()
{
	NodeName = "MoveToEgg";
	bCreateNodeInstance = true;
	bTickIntervals = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToEgg::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComponent = &OwnerComp;
	BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return EBTNodeResult::Failed;

	ARealYoshi* RY = Cast<ARealYoshi>(AIOwner->GetPawn());
	if (!RY)
	{
		return EBTNodeResult::Failed;
	}

	FVector RYLocation = RY->GetActorLocation();


	AActor* SelectedEgg = Cast<AActor>(BlackboardComponent->GetValueAsObject(TEXT("SelectedEgg")));
	if (!SelectedEgg) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToEgg::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
