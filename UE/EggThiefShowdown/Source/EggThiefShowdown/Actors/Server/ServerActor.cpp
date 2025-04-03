// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "UnrealClient.h"

// Sets default values
AServerActor::AServerActor()
{
    bReplicates = true;
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent0"));
    RootComponent = MeshComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshCube(
        TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    MeshComponent->SetStaticMesh(StaticMeshCube.Object);
}

// Called when the game starts or when spawned
void AServerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AServerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (HasAuthority())
    {
        AddActorWorldRotation(FRotator(0.f, 360.f * DeltaTime, 0.f));
        ActorRotation = GetActorRotation();
    }
}

void AServerActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // ActorRotation을 리플리케이션하도록 등록
    DOREPLIFETIME(ThisClass, ActorRotation);
}

void AServerActor::OnRep_Rotation()
{
    SetActorRotation(ActorRotation);
}

