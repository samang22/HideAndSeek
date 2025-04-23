// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Game/NPC/AI/BTTask_PathTraceWithRandomWait.h"
#include "Components/SplineComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/AIAsyncTaskBlueprintProxy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_PathTraceWithRandomWait::UBTTask_PathTraceWithRandomWait()
{
	NodeName = "PathTraceWithRandomWait";
	bCreateNodeInstance = true;
	bTickIntervals = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PathTraceWithRandomWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIOwner = OwnerComp.GetAIOwner();
	APawn* Pawn = AIOwner->GetPawn();

	if (Pawn && Pawn->HasAuthority())
	{
		BehaviorTreeComponent = &OwnerComp;
		BlackboardComponent = OwnerComp.GetBlackboardComponent();
		SplineComponent = Cast<USplineComponent>(BlackboardComponent->GetValueAsObject(TEXT("SplineComponent")));
		check(SplineComponent);

		if (SplineComponent)
		{
			SplinePoints = SplineComponent->GetNumberOfSplinePoints();
			CurrentPatrolIndex = BlackboardComponent->GetValueAsInt(TEXT("PatrolIndex"));
		}

		// 랜덤하게 정지 여부 결정 (50% 확률)
		bWaiting = FMath::FRand() < 0.5f;
		ElapsedTime = 0.0f;

		if (bWaiting)
		{
			// 정지 시간도 랜덤 (1~3초 사이)
			WaitTime = FMath::FRandRange(1.0f, 3.0f);
			return EBTNodeResult::InProgress;
		}
		else
		{
			// 바로 이동
			const FVector TargetLocation = SplineComponent->GetLocationAtSplinePoint(CurrentPatrolIndex, ESplineCoordinateSpace::World);
			Proxy = UAIBlueprintHelperLibrary::CreateMoveToProxyObject(AIOwner, Pawn, TargetLocation);
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
	}

	return EBTNodeResult::Failed;
}

void UBTTask_PathTraceWithRandomWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Pawn = AIOwner->GetPawn();
	if (!Pawn || !Pawn->HasAuthority()) return;

	UObject* DetectedEgg = BlackboardComponent->GetValueAsObject(TEXT("DetectedEgg"));
	if (IsValid(DetectedEgg))
	{
		AIOwner->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (bWaiting)
	{
		ElapsedTime += DeltaSeconds;
		if (ElapsedTime >= WaitTime)
		{
			// 대기 종료 후 이동 시작
			const FVector TargetLocation = SplineComponent->GetLocationAtSplinePoint(CurrentPatrolIndex, ESplineCoordinateSpace::World);
			Proxy = UAIBlueprintHelperLibrary::CreateMoveToProxyObject(AIOwner, Pawn, TargetLocation);
			if (Proxy)
			{
				Proxy->OnSuccess.AddDynamic(this, &ThisClass::OnResult);
				Proxy->OnFail.AddDynamic(this, &ThisClass::OnResult);
				bWaiting = false;
			}
			else
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
	}
}

void UBTTask_PathTraceWithRandomWait::OnResult(EPathFollowingResult::Type MovementResult)
{
	APawn* Pawn = AIOwner->GetPawn();

	if (!IsValid(Proxy) || !Pawn || !Pawn->HasAuthority())
	{
		return;
	}
	if (MovementResult == EPathFollowingResult::Success)
	{
		++CurrentPatrolIndex;
		CurrentPatrolIndex = CurrentPatrolIndex % SplinePoints;
		BlackboardComponent->SetValueAsInt(TEXT("PatrolIndex"), CurrentPatrolIndex);
	}

	FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
}
