// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Module5Proj/Player/PlayerCharacter.h"
#include "Module5Proj/Module5ProjProjectile.h"


// Sets default values
AAI_Character::AAI_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetMesh());

	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Loc"));
	MuzzleComponent->SetupAttachment(WeaponMesh);

	GunOffset = FVector(25.0f, 0.0f, 0.0f);

}

// Called when the game starts or when spawned
void AAI_Character::BeginPlay()
{
	Super::BeginPlay();
	
	//Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);
	//GetMesh()->HideBoneByName(TEXT("RightHand"), EPhysBodyOp::PBO_None);
	//Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	//Weapon->SetOwner(this);

	//UMaterialInstanceDynamic* const material_instace = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	//if (material_instace)
	//{
	//	/*material_instace->SetVectorParameterValue("BodyColor", FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	//	GetMesh()->SetMaterial(0, material_instace);*/
	//}
	
}

// Called every frame
void AAI_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAI_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APlayerCharacter* AAI_Character::GetPlayerCharacter() const
{
	return m_pPlayerCharacter;
}

USceneComponent* AAI_Character::GetMuzzleComponent()
{
	return MuzzleComponent;
}


void AAI_Character::Shoot()
{
	//if (Weapon)
	//{
	//	Weapon->PullTrigger();
	//}
	if (ProjectileClass != nullptr)
	 {
	 	UWorld* const World = GetWorld();
	 	if (World != nullptr)
	 	{

	 		const FRotator SpawnRotation = GetControlRotation(); 
	// 		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	 		const FVector SpawnLocation = ((GetMuzzleComponent() != nullptr) ? GetMuzzleComponent()->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	// 		//Set Spawn Collision Handling Override
	 		FActorSpawnParameters ActorSpawnParams;
	 		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// 		// spawn the projectile at the muzzle
	 		World->SpawnActor<AModule5ProjProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		}
	 }

	//if(ProjectileClass)
	//{

	//FActorSpawnParameters ActorSpawnParams;

	//const FVector kv3SpawnLocation = GetMuzzleComponent()->GetComponentLocation();

	//const FVector kv3Direction = (GetTargetLocation() - kv3SpawnLocation).GetSafeNormal();

	//GetWorld()->SpawnActor<AModule5ProjProjectile>(ProjectileClass, kv3SpawnLocation, kv3Direction.Rotation(), ActorSpawnParams);
	//}
}

void AAI_Character::KillEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("DED"));
}
