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



ARealYoshiAIController::ARealYoshiAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	// 시야 설정
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 900.0f;
	SightConfig->PeripheralVisionAngleDegrees = 120.0f;
	SightConfig->SetMaxAge(1.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// 감지 업데이트 이벤트 바인딩
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ARealYoshiAIController::OnPerceptionUpdated);

}
void ARealYoshiAIController::BeginPlay()
{
	Super::BeginPlay();

	// Behavior Tree 실행 및 Blackboard 초기화 (Possess 시점으로 변경 가능)
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ControlledPawn->HasAuthority())
		{
			UBehaviorTree* BehaviorTree = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/Blueprint/NPC/BT_RealYoshi.BT_RealYoshi"));
			if (BehaviorTree)
			{
				RunBehaviorTree(BehaviorTree);
				UE_LOG(LogTemp, Warning, TEXT("RunBehaviorTree(BehaviorTree) in BeginPlay"));
			}
		}
	}
}

void ARealYoshiAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Blackboard 초기화 (BeginPlay에서 처리하도록 변경)
	if (HasAuthority() && Blackboard)
	{
		Blackboard->SetValueAsObject(TEXT("SplineComponent"), PatrolPath.Get());
	}
}



void ARealYoshiAIController::SetPatrolPath(TObjectPtr<USplineComponent> NewPatrolPath, APawn* OwningPawn)
{
	Super::SetPatrolPath(NewPatrolPath);

	if (IsValid(OwningPawn) && OwningPawn->HasAuthority())
	{
		if (!IsValid(NewPatrolPath))
		{
			return;
		}

		UBehaviorTree* BehaviorTree = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/Blueprint/NPC/BT_RealYoshi.BT_RealYoshi"));
		check(BehaviorTree);

		// Blackboard 인스턴스화
		UBlackboardComponent* MyBlackboard = nullptr;
		if (UseBlackboard(BehaviorTree->BlackboardAsset, MyBlackboard))
		{
			Blackboard = MyBlackboard;  // 이 컨트롤러의 인스턴스에 할당
		}

		RunBehaviorTree(BehaviorTree);

		if (Blackboard)
		{
			Blackboard->SetValueAsObject(TEXT("SplineComponent"), NewPatrolPath);
		}

		UE_LOG(LogTemp, Warning, TEXT("RunBehaviorTree(BehaviorTree)"));
	}
}

void ARealYoshiAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (HasAuthority() && Blackboard)
	{
		AEgg* MostRecentEgg = nullptr;

		for (AActor* SeenActor : UpdatedActors)
		{
			if (AEgg* DetectedEgg = Cast<AEgg>(SeenActor))
			{
				if (!DetectedEgg->IsCoolTime() && !DetectedEgg->GetIsHold())
				{
					MostRecentEgg = DetectedEgg;
					break; // 일단 하나만 감지하면 멈춤 (필요에 따라 로직 변경 가능)
				}
			}
		}

		if (MostRecentEgg)
		{
			Blackboard->SetValueAsObject(TEXT("DetectedEgg"), Cast<UObject>(MostRecentEgg));
		}
		else
		{
			Blackboard->ClearValue(TEXT("DetectedEgg"));
		}
	}
}