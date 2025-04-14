// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MarioAttack.h"
#include "Actors/Game/Character/GamePlayer.h"
#include "Actors/Game/Projectile/Projectile.h"
#include "Misc/Utils.h"

UAnimNotify_MarioAttack::UAnimNotify_MarioAttack()
{
}

void UAnimNotify_MarioAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
#if WITH_EDITOR
	if (GIsEditor && MeshComp->GetWorld() != GWorld) { return; } // 에디터 프리뷰
#endif


	if (AGamePlayer* GP = Cast<AGamePlayer>(MeshComp->GetOwner()))
	{
		FVector Location = GP->GetActorLocation();

		const FVector ForwardVector = GP->GetActorForwardVector();
		UWorld* World = GP->GetWorld();

		AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
			FTransform::Identity, GP, GP, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FTransform NewTransform;
		Projectile->SetData(ProjectileName::MarioAttack, CollisionProfileName::MarioAttack);
		NewTransform.SetLocation(GP->GetActorLocation() + MARIOATTACK_LENGTH * ForwardVector);
		NewTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
		Projectile->FinishSpawning(NewTransform);
	}
}
