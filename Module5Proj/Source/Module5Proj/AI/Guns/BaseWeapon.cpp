// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Module5Proj/Module5ProjProjectile.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(Mesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	GunOffset = FVector(100.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USceneComponent* ABaseWeapon::GetMuzzleComponent()
{
	return MuzzleLocation;
}

void ABaseWeapon::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	// try and fire a projectile
	//if (ProjectileClass != nullptr)
	//{
	//	UWorld* const World = GetWorld();
	//	if (World != nullptr)
	//	{
	//		const FRotator SpawnRotation = GetActorRotation();
	//		UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *SpawnRotation.ToString());
	//		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	//		const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	//		//Set Spawn Collision Handling Override
	//		FActorSpawnParameters ActorSpawnParams;
	//		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	//		// spawn the projectile at the muzzle
	//		World->SpawnActor<AModule5ProjProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

	//	}
	//}
	FActorSpawnParameters ActorSpawnParams;

	const FVector kv3SpawnLocation = GetMuzzleComponent()->GetComponentLocation();

	const FVector kv3Direction = (GetTargetLocation() - kv3SpawnLocation).GetSafeNormal();

	GetWorld()->SpawnActor<AModule5ProjProjectile>(ProjectileClass, kv3SpawnLocation, kv3Direction.Rotation(), ActorSpawnParams);

}
