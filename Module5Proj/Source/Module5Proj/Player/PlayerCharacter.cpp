// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../Module5ProjProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PlayerMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "DrawDebugHelpers.h"
#include "Module5Proj/AI/AI_Character.h"
#include "Camera/CameraComponent.h"
#include "Module5Proj/Player/Components/Camera/PlayerCameraComponent.h"
#include "Module5Proj/Player/Components/AbilityComponent.h"
#include "Module5Proj/Player/Abilities/Ability_PositionSwap.h"
#include "Module5Proj/Player/Abilities/Ability_Shield.h"
#include "Module5Proj/Player/Components/MeleeComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// APlayerCharacter

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Combat Collision"));
	CombatCollision->SetupAttachment(FirstPersonCameraComponent);
	//CombatCollision->SetCollisionProfileName("Trigger");

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	//CombatCollision->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	//m_ACAbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));

	m_ACAbilitySwapPos = CreateDefaultSubobject<UAbility_PositionSwap>(TEXT("Ability_SwapPos"));
	
	m_ACAbilityShield =	 CreateDefaultSubobject<UAbility_Shield>(TEXT("Ability_Shield"));

	m_ACMeleeComponent =  CreateDefaultSubobject<UMeleeComponent>(TEXT("Melee Component"));

	m_bPressedSprint = false;
	bPressedCrouch = false;
	
	setUpStimulus();

	m_fJumpHeight = 600.f;

	m_bCanDash = true;
	m_fDashDistance = 6000.f;
	m_fDashCooldown = 2.f;

	m_bCanAttack = true;
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPlayerMovementComponent* pPlayerMovementComponent = Cast<UPlayerMovementComponent>(GetCharacterMovement());
	if (pPlayerMovementComponent)
	{
		m_ACPlayerMovementComponent = pPlayerMovementComponent;
	}

}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("SwordSocket"));

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverLapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("CanAttack: %s"), m_bCanAttack ? TEXT("true") : TEXT("false")));
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), m_bPressedSprint ? TEXT("true") : TEXT("false")));
		//GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Blue, FString::SanitizeFloat(GetFirstPersonCameraComponent()->FieldOfView));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::DoubleJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::Dash);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::OnSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StoppedPressed);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::OnCrouch);

	PlayerInputComponent->BindAction("Ability_TimeSlow", IE_Pressed, this, &APlayerCharacter::OnAbility1);

	PlayerInputComponent->BindAction("Ability_SwapPos", IE_Pressed, this, &APlayerCharacter::OnAbility2Used);

	PlayerInputComponent->BindAction("Mouse_Right", IE_Pressed, this, &APlayerCharacter::OnAbilityCancel);

	PlayerInputComponent->BindAction("Ability_Shield", IE_Pressed, this, &APlayerCharacter::OnAbility3Used);

}

void APlayerCharacter::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		AAI_Character* Enemy = Cast<AAI_Character>(OtherActor);
		if(Enemy)
		{
			if(Enemy->m_psBloodEffect && HitSound)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->m_psBloodEffect, Enemy->GetActorLocation(), FRotator::ZeroRotator, false);
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, Enemy->GetActorLocation());
			}
		}
	}
}

void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{


	}
}

void APlayerCharacter::OnFire()
{
	// // try and fire a projectile
	// if (ProjectileClass != nullptr)
	// {
	// 	UWorld* const World = GetWorld();
	// 	if (World != nullptr)
	// 	{

	// 		const FRotator SpawnRotation = GetControlRotation(); 
	// 		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	// 		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	// 		//Set Spawn Collision Handling Override
	// 		FActorSpawnParameters ActorSpawnParams;
	// 		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// 		// spawn the projectile at the muzzle
	// 		World->SpawnActor<AModule5ProjProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			
	// 	}
	// }
	// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr && FireSound != nullptr && m_bCanAttack)
		{
			//GetWorld()->GetTimerManager().SetTimer(TimerAttacks, this, &APlayerCharacter::DecreaseAbilitySize, m_fAbilityDecreaseRate, true);
			m_bCanAttack = false;
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			//UGameplayStatics::SpawnEmitterAttached(m_pShieldParticleSystem, GetMesh1P(), FName("ShieldSocket"), FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), EAttachLocation::KeepWorldPosition, true);
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	m_iJumpCounter = 0;
}

void APlayerCharacter::DoubleJump()
{
	if(m_iJumpCounter <= 1)
	{
		APlayerCharacter::LaunchCharacter(FVector(0, 0, m_fJumpHeight), false, true);
		m_iJumpCounter++;
	}
}

void APlayerCharacter::Dash()
{
	if(m_bCanDash)
	{
		GetCharacterMovement()->BrakingFriction = 0.f;
		const FVector ForwardDir = this->GetActorRotation().Vector();
		LaunchCharacter(ForwardDir * m_fDashDistance, true, true);

		//UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetVelocity().Normalize() * m_fDashDistance;
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
	
	if (Value == 0.0f)
	{
		if (m_ACPlayerMovementComponent)
		{
			m_ACPlayerMovementComponent->StopSprinting();
		}
	}
			
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}

	/*if (Value == 0.0f)
	{
		if (m_ACPlayerMovementComponent)
		{
			m_ACPlayerMovementComponent->StopSprinting();
		}
	}*/

}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::OnSprint()
{
	m_bPressedSprint = true;
	if(m_ACPlayerMovementComponent)
	{
		m_ACPlayerMovementComponent->StartSprinting();
	}
 
}

void APlayerCharacter::StoppedPressed()
{
	//m_bPressedSprint = false;
}

void APlayerCharacter::OnCrouch()
{
	if (m_ACPlayerMovementComponent)
	{
		if (!bPressedCrouch)
		{
			bPressedCrouch = true;
			m_ACPlayerMovementComponent->StartCrouching();
			//UE_LOG(LogTemp, Warning, TEXT("START CROUCH"));
		}
		else
		{
			bPressedCrouch = false;
			m_ACPlayerMovementComponent->UnCrouching();
			//UE_LOG(LogTemp, Warning, TEXT("STOP CROUCH"));
		}
	}

	/*UE_LOG(LogTemp, Warning, TEXT("ALLO"));
	if (!GetCharacterMovement()->IsCrouching())
	{
		GetCharacterMovement()->bWantsToCrouch = true;
		GetCharacterMovement()->Crouch();
	}
	else
	{
		GetCharacterMovement()->bWantsToCrouch = false;
		GetCharacterMovement()->UnCrouch();
	}*/
}

bool APlayerCharacter::GetPressedSprint()
{
	return m_bPressedSprint;
}

void APlayerCharacter::setUpStimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

void APlayerCharacter::OnAbility1()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25);

	GetActorTimeDilation();

}

void APlayerCharacter::OnAbility2Used()
{

	if(!m_ACAbilitySwapPos->GetAbilityActive())
	{
		m_ACAbilitySwapPos->AbilityActivated();
	}
	else
	{
		//FirstPersonCameraComponent->PostProcessSettings.ColorGamma = FVector4(1, 1, 1, 1);
		m_ACAbilitySwapPos->AbilityUsed();
	}

}

void APlayerCharacter::OnAbilityCancel()
{
	if(m_ACAbilitySwapPos->GetAbilityActive())
	{
		FirstPersonCameraComponent->PostProcessSettings.ColorGamma = FVector4(1, 1, 1, 1);
		m_ACAbilitySwapPos->AbilityCancelled();
	}
}

void APlayerCharacter::OnAbility3Used()
{
	if(m_ACAbilityShield)
	{
		m_ACAbilityShield->AbilityUsed();
	}
}

void APlayerCharacter::Die()
{
	if(!m_ACAbilityShield->GetShieldActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("DED"));
	}

}
