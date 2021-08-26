// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include <Components/CapsuleComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Module5Proj/Player/PlayerCharacter.h"
#include <DrawDebugHelpers.h>
#include "Engine/Engine.h"


// Sets default values for this component's properties
UPlayerMovementComponent::UPlayerMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_fSprindSpeed = 1000.f;
	m_fCrouchSpeed = 300.f;

	AirControl = 0.25f;
	AirControlBoostMultiplier = 2;
}


// Called when the game starts
void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pPlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (m_pPlayerCharacter)
	{
		m_pCameraComponent = m_pPlayerCharacter->GetFirstPersonCameraComponent();
	}

	// Set Standing Capsule Half Height
	m_fStandingCapsuleHalfHeight = m_pPlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	// Set Relative Z Offset on Camera
	//m_fStandingCameraZOffSet = m_pPlayerCharacter->GetFirstPersonCameraComponent()->GetRelativeLocation().Z;

	if (m_pCapsuleHalfHeightCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		m_CrouchingTimeline.AddInterpFloat(m_pCapsuleHalfHeightCurve, TimelineProgress);
		m_CrouchingTimeline.SetLooping(false);
	}
	
}

void UPlayerMovementComponent::TimelineProgress(float fTransitionProgress)
{
	float fNewHalfHeight = FMath::Lerp(m_fStandingCapsuleHalfHeight, m_fCrouchingCapsuleHalfHeight, fTransitionProgress);

	m_pPlayerCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(fNewHalfHeight);

	float fNewCameraRelativeZ = FMath::Lerp(m_fStandingCameraZOffSet, m_fCrouchingCameraZOffset, fTransitionProgress);

	//FVector v3CameraRelativeLocation = m_pCameraComponent->GetRelativeLocation();
	//v3CameraRelativeLocation.Z = fNewCameraRelativeZ;
	//m_pCameraComponent->SetRelativeLocation(v3CameraRelativeLocation);
}

// Called every frame
void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_CrouchingTimeline.IsPlaying())
	{
		m_CrouchingTimeline.TickTimeline(DeltaTime);
	}

	if ((eMovementState == EMovementState::Crouching) && (CanStand()))
	{
		ResolveMovement();
	}

	const TEnumAsByte<EMovementState> SurfaceEnum = eMovementState;
	FString EnumAsString = UEnum::GetValueAsString(SurfaceEnum.GetValue());

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Cyan, FString::SanitizeFloat(MaxWalkSpeed));
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Red, EnumAsString);
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), m_pPlayerCharacter->GetPressedSprint() ? TEXT("true") : TEXT("false")));

	}

}

void UPlayerMovementComponent::StartSprinting()
{
	StartMovementStateSwitch(EMovementState::Sprinting);
}

void UPlayerMovementComponent::StopSprinting()
{	
	bisSprinting = false;
	StartMovementStateSwitch(EMovementState::Walking);
	ResolveMovement();
	MaxWalkSpeed = 600.f;
}

void UPlayerMovementComponent::StartCrouching()
{
	if (eMovementState == EMovementState::Walking || eMovementState == EMovementState::Sprinting)
	{
		StartMovementStateSwitch(EMovementState::Crouching);
	}
	else if (eMovementState == EMovementState::Sprinting)
	{
		//StartMovementStateSwitch(EMovementState::Sliding);
	}

}

void UPlayerMovementComponent::Crouching()
{
	if (!bisCrouching)
	{
		m_CrouchingTimeline.Play();
		//bisCrouching = true;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Began Crouch"));
		}
	}
}

void UPlayerMovementComponent::UnCrouching()
{
	if (bisCrouching)
	{
		m_CrouchingTimeline.Reverse();
		bisCrouching = false;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Stopped Crouch"));
		}
	}
}

void UPlayerMovementComponent::StartSlide()
{
	
}

void UPlayerMovementComponent::StopSlide()
{

}

void UPlayerMovementComponent::ResolveMovement()
{
	if (CanSprint())
	{
		StartMovementStateSwitch(EMovementState::Sprinting);
	}
	else
	{
		if (CanStand())
		{
			StartMovementStateSwitch(EMovementState::Walking);
		}
		else
		{
			StartMovementStateSwitch(EMovementState::Crouching);
		}
	}

}

void UPlayerMovementComponent::StartMovementStateSwitch(EMovementState eNewMovementState) //InitiateMovementStateSwitch
{
	if (eNewMovementState != eMovementState)
	{
		SwitchMovementState(eNewMovementState);
	}
}

void UPlayerMovementComponent::SwitchMovementState(EMovementState& eNewMovementState) //SwitchMovementState
{
	SetMovementState(eNewMovementState);
	eMovementState = eNewMovementState;
}

void UPlayerMovementComponent::SetMovementState(EMovementState& eNewMovementState) //OnEnterMovementState
{
	switch (eNewMovementState)
	{
	case EMovementState::Walking:
	{
		//UnCrouching();
		//bisSprinting = false;
		m_pPlayerCharacter->SetPressedSprint(false);
	}
		break;
	case EMovementState::Sprinting:
	{
		bisSprinting = true;
		MaxWalkSpeed = m_fSprindSpeed;
		m_pPlayerCharacter->bPressedCrouch = false;
		UnCrouching();
	}
		break;
	case EMovementState::Crouching:
	{
		MaxWalkSpeed = m_fCrouchSpeed;
		Crouching();
		bisSprinting = false;
		bisCrouching = true;
	}
	break;
	case EMovementState::Sliding:
	{
		Crouching();
		StartSlide();
	}
		break;
	case EMovementState::Jumping:
	{
		//m_bIsSprinting = false;
	}
	break;
	default:
		break;
	}
}

void UPlayerMovementComponent::OnMovementStateChange(EMovementState& eNewMovementState)
{

}

bool UPlayerMovementComponent::CanSprint() const
{
	if (!m_pPlayerCharacter->GetPressedSprint())
	{
		return false;
	}
	else
	{
		if (CanStand() && !IsFalling())
		{
			return true;
		}
	}
	return true;
}

bool UPlayerMovementComponent::CanStand() const
{
	if (bisCrouching)
	{
		return false;
	}

	if ((eMovementState != EMovementState::Crouching && eMovementState != EMovementState::Sliding))
	{
		return true;
	}

	//UCapsuleComponent* PlayerCapsuleComponent = m_pPlayerCharacter->GetCapsuleComponent();
	//if (nullptr != PlayerCapsuleComponent)
	//{
	//	// Define CapsuleTraceLocation
	//	// This will be the centre location of the Walking state player capsule
	//	// 1. Get location of centre of small crouched capsule 
	//	FVector CapsuleTraceLocation = m_pPlayerCharacter->GetActorLocation();
	//	// 2. Subtract half the height, get location of bottom of the capsule
	//	CapsuleTraceLocation.Z -= PlayerCapsuleComponent->GetScaledCapsuleHalfHeight();
	//	// 3. Add the standing half height, we have reached the centre of the standing capsule
	//	CapsuleTraceLocation.Z += m_fStandingCapsuleHalfHeight;

	//	bool bSpaceOccluded = CheckCapsuleCollision(CapsuleTraceLocation, m_fStandingCapsuleHalfHeight, PlayerCapsuleComponent->GetScaledCapsuleRadius(), true);
	//	return !bSpaceOccluded;
	//}
	return true;
	
}

bool UPlayerMovementComponent::CheckCapsuleCollision(FVector Center, float HalfHeight, float Radius, bool DrawDebug)const
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
	ObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(m_pPlayerCharacter);
	UClass* ActorClassFilter = AActor::StaticClass();
	TArray<AActor*> ActorsFound;
	bool bOverlapDetected = UKismetSystemLibrary::CapsuleOverlapActors(GetWorld(), Center, Radius, HalfHeight, ObjectTypeQuery, ActorClassFilter, ActorsToIgnore, ActorsFound);

	if (DrawDebug)
	{
		FColor DebugColour = FColor::Red;
		if (!bOverlapDetected)
		{
			DebugColour = FColor::Green;
		}
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, Radius, FRotator(0.f, 0.f, 0.f).Quaternion(), DebugColour, false, 2.0f);
	}
	return bOverlapDetected;
}

void UPlayerMovementComponent::TEST()
{
	UE_LOG(LogTemp, Warning, TEXT("?D?SADSA"));
}