// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Components/TimelineComponent.h>
#include "PlayerMovementComponent.generated.h"

class APlayerCharacter;
class UCameraComponent;

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	//Stopped		UMETA(DisplayName = "Stopped"),
	Walking		UMETA(DisplayName = "Walking"),
	Jumping		UMETA(DisplayName = "Jumping"),
	Sprinting	UMETA(DisplayName = "Sprinting"),
	Crouching	UMETA(DisplayName = "Crouching"),
	Sliding		UMETA(DisplayName = "Sliding"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MODULE5PROJ_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerMovementComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartSprinting();

	void StopSprinting();

	void StartCrouching();

	void UnCrouching();

	void Crouching();

	void StartSlide();

	void StopSlide();

	void ResolveMovement();

	void SetMovementState(EMovementState& eNewMovementState);

	void OnMovementStateChange(EMovementState& eNewMovementState);

	void SwitchMovementState(EMovementState& eNewMovementState);

	void StartMovementStateSwitch(EMovementState eNewMovementState);

	void CalculateFloorInfluence();

	bool CanSprint() const;

	bool CanStand() const;

	bool CheckCapsuleCollision(FVector Center, float HalfHeight, float Radius, bool DrawDebug)const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FTimeline m_CrouchingTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline | Capsule Scale Down")
	UCurveFloat* m_pCapsuleHalfHeightCurve;

	//UPROPERTY(EditAnywhere, Category = "Slide | Sliding Camera Tilt")
	//UCurveFloat* m_pCameraTiltCurve;

public:

	UFUNCTION()
		void TimelineProgress(float fTransitionProgress);

	EMovementState eMovementState;

	bool bisSprinting;
	bool bisCrouching;

	float m_fStandingCapsuleHalfHeight;
	float m_fStandingCameraZOffSet;

	float SetSpeed() const;

	APlayerCharacter* m_pPlayerCharacter;
	UCameraComponent* m_pCameraComponent;

private:

	UPROPERTY(Category = "Player Capsule Size", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "10.0", ClampMax = "60.0", AllowPrivateAccess = "true"))
		float m_fCrouchingCapsuleHalfHeight;

	UPROPERTY(Category = "Player Camera", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "10.0", ClampMax = "100.0", AllowPrivateAccess = "true"))
		float m_fCrouchingCameraZOffset;

	UPROPERTY(Category = "Movement | Sprint | Speed", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), DisplayName = "Sprint Speed")
		float m_fSprindSpeed;

	UPROPERTY(Category = "Movement | Crouch | Speed", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), DisplayName = "Sprint Speed")
		float m_fCrouchSpeed;

};
