// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MarioPickup.h"
#include "Actors/Game/Character/GamePlayer.h"
#include "Actors/Game/Projectile/Projectile.h"
#include "Misc/Utils.h"
UAnimNotify_MarioPickup::UAnimNotify_MarioPickup()
{
}

void UAnimNotify_MarioPickup::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
#if WITH_EDITOR
	if (GIsEditor && MeshComp->GetWorld() != GWorld) { return; } // 에디터 프리뷰
#endif

	if (AGamePlayer* GP = Cast<AGamePlayer>(MeshComp->GetOwner()))
	{
		if (GP->GetCharacterKind() == LOBBY_CHARACTER_KIND::MARIO)
		{
			GP->SetCanMove(false);

			if (!GP->GetIsEgg())
			{
				const FVector GPLocation = GP->GetActorLocation();

				const FVector ForwardVector = GP->GetActorForwardVector();
				UWorld* World = GP->GetWorld();

				AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
					FTransform::Identity, GP, GP, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

				FTransform NewTransform;
				Projectile->SetData(ProjectileName::MarioPickup, CollisionProfileName::MarioPickup);
				NewTransform.SetLocation(GPLocation + MARIOATTACK_LENGTH * ForwardVector);
				NewTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
				Projectile->FinishSpawning(NewTransform);
			}
		}
	}
}
