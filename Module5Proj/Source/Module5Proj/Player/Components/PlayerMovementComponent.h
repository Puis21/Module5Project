// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

class APlayerCharacter;

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walking		UMETA(DisplayName = "Walking"),
	Jumping		UMETA(DisplayName = "Jumping"),
	Sprinting	UMETA(DisplayName = "Sprinting"),
	Crouching	UMETA(DisplayName = "Crouching"),
	Sliding		UMETA(DisplayName = "Sliding"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MODULE5PROJ_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMovementComponent();

	UPROPERTY(Category = "Movement | Sprint | Speed", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), DisplayName = "Sprint Speed")
	float m_fSprindSpeed;


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartSprinting();

	void StopSprinting();

	void StartCrouch();

	void StartSlide();

	void StopSlide();

	void ResolveMovement();

	void SetMovementState(EMovementState& eNewMovementState);

	void OnMovementStateChange(EMovementState& eNewMovementState);

	void SwitchMovementState(EMovementState& eNewMovementState);

	void StartMovementStateSwitch(EMovementState eNewMovementState);

	void CalculateFloorInfluence();

	void CanSprint();

	void CanStand();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	EMovementState eMovementState;

	bool bisSprinting;
	bool bisCrouching;

	float fStandingCapsuleHalfHeight;
	float fStandingCameraZOffSet;
		
};
