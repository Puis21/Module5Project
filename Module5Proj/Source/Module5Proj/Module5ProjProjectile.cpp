// Copyright Epic Games, Inc. All Rights Reserved.

#include "Module5ProjProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Module5Proj/Player/PlayerCharacter.h"
#include "Module5Proj/AI/AI_Character.h"

AModule5ProjProjectile::AModule5ProjProjectile() 
{
	//MeshBullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Bullet"));
	//MeshBullet->SetupAttachment(MeshBullet);

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(20.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AModule5ProjProjectile::OnOverLapBegin);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0;

	// Die after 3 seconds by default
	InitialLifeSpan = 10.0f;
}

void AModule5ProjProjectile::BeginPlay()
{
	Super::BeginPlay();

}


void AModule5ProjProjectile::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor);
		AAI_Character* Enemy = Cast<AAI_Character>(OtherActor);
		if(PlayerChar)
		{
			Destroy();
			PlayerChar->Die();
		}

		if(Enemy)
		{
			Destroy();
			Enemy->KillEnemy();
		}

		Destroy();
	}
}