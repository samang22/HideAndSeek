// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_YoshiPickup.h"
#include "Actors/Game/Character/GamePlayer.h"
#include "Actors/Game/Projectile/Projectile.h"
#include "Actors/Game/NPC/RealYoshi.h"
#include "Misc/Utils.h"

UAnimNotify_YoshiPickup::UAnimNotify_YoshiPickup()
{
}

void UAnimNotify_YoshiPickup::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
#if WITH_EDITOR
	if (GIsEditor && MeshComp->GetWorld() != GWorld) { return; } // 에디터 프리뷰
#endif


	if (ARealYoshi* RY = Cast<ARealYoshi>(MeshComp->GetOwner()))
	{
		RY->StopMovement();

	}
	else if (AGamePlayer* GP = Cast<AGamePlayer>(MeshComp->GetOwner()))
	{
		if (GP->GetCharacterKind() == LOBBY_CHARACTER_KIND::YOSHI)
		{
			GP->SetCanMove(false);

			if (GP->GetIsEgg())
			{
				if (GP->HasAuthority())
				{
					GP->DropEgg();
				}
				else
				{
					GP->Server_DropEgg();
				}
			}
			else
			{
				const FVector GPLocation = GP->GetActorLocation();

				const FVector ForwardVector = GP->GetActorForwardVector();
				UWorld* World = GP->GetWorld();

				AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
					FTransform::Identity, GP, GP, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

				FTransform NewTransform;
				Projectile->SetData(ProjectileName::YoshiPickup, CollisionProfileName::YoshiPickup);
				NewTransform.SetLocation(GPLocation + MARIOATTACK_LENGTH * ForwardVector);
				NewTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
				Projectile->FinishSpawning(NewTransform);
			}
		}
	}

}
