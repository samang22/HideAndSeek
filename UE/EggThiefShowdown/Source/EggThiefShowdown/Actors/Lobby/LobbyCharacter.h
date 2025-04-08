// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Misc/Utils.h"
#include "GameFramework/Actor.h"
#include "LobbyCharacter.generated.h"

class USkeletalMeshComponent;
class ULobbyCharacterStatusComponent;
class UWidgetComponent;
class ULobbySelectCharacterButtonWidget;
struct FLobbyCharacterTableRow;





UCLASS()
class EGGTHIEFSHOWDOWN_API ALobbyCharacter : public AActor
{
	GENERATED_BODY()

	virtual void PostInitProperties() override;
public:	
	// Sets default values for this actor's properties
	ALobbyCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
	virtual void PostLoad() override;
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	virtual void OnConstruction(const FTransform& Transform);
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULobbyCharacterStatusComponent> StatusComponent;
public:
	TObjectPtr<ULobbyCharacterStatusComponent> GetStatusComponent() { return StatusComponent; }
	LOBBY_CHARACTER_KIND GetLobbyCharacterKind() const;
	

protected:
	TObjectPtr<UWidgetComponent> SelectButtonWidgetComponent;
	LOBBY_CHARACTER_SELECT_BUTTON_STATE eSelectButtonStatus;

public:
	LOBBY_CHARACTER_SELECT_BUTTON_STATE GetSelectionState(const FString& InUserName);


protected:
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle DataTableRowHandle;
	FLobbyCharacterTableRow* LobbyCharacterData;


public:
	void PlayMontage(LOBBY_CHARACTER_MONTAGE _InEnum, bool bIsLoop = false);
	bool IsMontage(LOBBY_CHARACTER_MONTAGE _InEnum);
	bool IsPlayingMontage(LOBBY_CHARACTER_MONTAGE _InEnum);


protected:
	UPROPERTY(Replicated)
	FString UserName = TEXT("");

public:
	UFUNCTION()
	void SetUserName(const FString& InUserName);
	FString GetUserName() const { return UserName; }


public:
	// 액터 선택 함수
	UFUNCTION()
	void SelectActor(const FString& InUserName);

	// 선택 상태가 변경되었을 때 호출되는 Replication 함수
	UFUNCTION()
	void OnRep_SelectedPlayerID();

	// RPC: 서버에서 선택 상태를 업데이트하는 함수
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSelectActor(const FString& InUserName);

	// Multicast: 서버에서 상태를 업데이트한 후 모든 클라이언트에게 전달
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateActorSelection(const FString& InUserName);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
