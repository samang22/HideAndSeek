// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../Misc/Utils.h"
#include "GameFramework/Character.h"
#include "GamePlayer.generated.h"

class USkeletalMeshComponent;
class UGamePlayerStatusComponent;
class UCameraComponent;
class USoftWheelSpringArmComponent;
class USpringArmComponent;
struct FGamePlayerTableRow;

UCLASS()
class EGGTHIEFSHOWDOWN_API AGamePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGamePlayer(const FObjectInitializer& ObjectInitializer);
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
	virtual void PostLoad() override;
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	virtual void OnConstruction(const FTransform& Transform);
	virtual void PostInitializeComponents() override;

public:
	/** Called when Controller is replicated */
	virtual void OnRep_Controller() override;

protected:
	void InitDataTableByPlayerState();
	void SetInputModeGameOnly();           

protected:
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USoftWheelSpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_UpdateDataTableRowHandle();

	UPROPERTY(ReplicatedUsing = OnRep_UpdateDataTableRowHandle, EditAnywhere,  meta = (RowType = "GamePlayerData"))
	FDataTableRowHandle DataTableRowHandle;
	FGamePlayerTableRow* GamePlayerData = nullptr;


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGamePlayerStatusComponent> StatusComponent;
public:
	TObjectPtr<UGamePlayerStatusComponent> GetStatusComponent() { return StatusComponent; }

public:
	void PlayMontage(GAME_PLAYER_MONTAGE _InEnum, bool bIsLoop = false);
	UFUNCTION(Server, Reliable)
	void Server_PlayMontage(uint8 _InEnum, bool bIsLoop = false);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayMontage(uint8 _InEnum, bool bIsLoop = false);


	bool IsMontage(GAME_PLAYER_MONTAGE _InEnum);
	bool IsPlayingMontage(GAME_PLAYER_MONTAGE _InEnum);

protected:
	void SetSpeedWalk();
	void SetSpeedRun();
protected:
	bool bIsRun = false;
public:
	bool GetIsRun() const { return bIsRun; }


protected:
	UPROPERTY(ReplicatedUsing = OnRep_MaxWalkSpeed)
	float ReplicatedMaxWalkSpeed;

public:
	UFUNCTION(Server, Reliable)
	void Server_SetSpeedWalk();
	UFUNCTION(Server, Reliable)
	void Server_SetSpeedRun();

	UFUNCTION()
	void OnRep_MaxWalkSpeed();

public:
	LOBBY_CHARACTER_KIND GetCharacterKind();

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	bool bCanMove = true;
public:
	void SetCanMove(bool bFlag) { bCanMove = bFlag; };

};
