// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Misc/Utils.h"
#include "RealYoshi.generated.h"

class APatrolPath;
class UAdvancedFloatingPawnMovement;
class URealYoshiStatusComponent;
struct FGamePlayerTableRow;

UCLASS()
class EGGTHIEFSHOWDOWN_API ARealYoshi : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARealYoshi(const FObjectInitializer& ObjectInitializer);
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
	virtual void PostLoad() override;
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform);
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected: // 옵션에 따라 생성되고 Root로 지정 됩니다.
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShapeComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	UAdvancedFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	URealYoshiStatusComponent* StatusComponent;

protected:
	UFUNCTION()
	void OnRep_UpdateDataTableRowHandle();
public:
	UPROPERTY(ReplicatedUsing = OnRep_UpdateDataTableRowHandle, EditAnywhere/*, meta = (RowType = "FGamePlayerTableRow")*/)
	FDataTableRowHandle DataTableRowHandle;
	FGamePlayerTableRow* RealYoshiData = nullptr;

public:
	const FGamePlayerTableRow* GetRealYoshiData() const { return RealYoshiData; }

protected:
	void PlayMontage(GAME_PLAYER_MONTAGE _InEnum, bool bIsLoop = false);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayMontage(uint8 _InEnum, bool bIsLoop = false);

public:
	UFUNCTION(Server, Reliable)
	void Server_PlayMontage(uint8 _InEnum, bool bIsLoop = false);
	bool IsMontage(GAME_PLAYER_MONTAGE _InEnum);
	bool IsPlayingMontage(GAME_PLAYER_MONTAGE _InEnum);



protected:
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere)
	class UAISenseConfig_Sight* AISenseConfig_Sight;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<APatrolPath> PatrolPathRef;


public:
	URealYoshiStatusComponent* GetStatusComponent() { return StatusComponent; }


protected:
	void TickMovement(float DeltaTime);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	bool bIsMoveToEgg = false;
	FVector MoveToEggDirection = FVector::Zero();
public:
	void SetMoveToEggWithDirection(FVector _Direction);
	void StopMovement();
	void ResumeMovement();
};
