// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/NPC/AI/BTTask_CareEgg.h"
#include "AIController.h"
#include "Actors/Game/NPC/RealYoshi.h"         
#include "Actors/Game/NPC/Egg.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CareEgg::UBTTask_CareEgg()
{
	NodeName = "CareEgg";
	bCreateNodeInstance = true;
	bTickIntervals = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CareEgg::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIOwner = OwnerComp.GetAIOwner();

	APawn* Pawn = AIOwner->GetPawn();

	if (Pawn && Pawn->HasAuthority())
	{
		ARealYoshi* RY = Cast<ARealYoshi>(Pawn);
		
		// @TODO : 적당한 애니메이션으로 교체
		RY->Server_PlayMontage((uint8)GAME_PLAYER_MONTAGE::PICKUP);
		RY->StopMovement();

		BlackboardComponent = OwnerComp.GetBlackboardComponent();
		if (AEgg* Egg = Cast<AEgg>(BlackboardComponent->GetValueAsObject(TEXT("DetectedEgg"))))
		{
			Egg->SetCoolTimeZero();
			BlackboardComponent->ClearValue(TEXT("DetectedEgg"));
		}
		
	}

	return EBTNodeResult::Succeeded;
}

