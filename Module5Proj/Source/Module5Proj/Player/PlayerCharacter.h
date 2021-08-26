// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UPlayerCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UPlayerMovementComponent;
class UMeleeComponent;
class UAbilityComponent;
class UAbility_PositionSwap;
class UAbility_Shield;

UCLASS(config=Game)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	///** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//UPlayerCameraComponent* FirstPersonCameraComponent;

protected:

	virtual void BeginPlay();

	virtual void Tick(float Deltatime);

public:

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Collision)
	class UBoxComponent* CombatCollision;

	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AModule5ProjProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Sounds)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	USoundBase* HitSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* FireAnimation;

protected:
	
	UFUNCTION()
	void OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void OnFire();

	void OnSprint();

	void OnCrouch();

	void StoppedPressed();

	void OnAbility1();

	void OnAbility2Used();

	void OnAbilityCancel();

	void OnAbility3Used();

	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void Landed(const FHitResult& Hit) override;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent	() const { return FirstPersonCameraComponent; }

	UPlayerMovementComponent* GetPlayerMovementComponent() const {return m_ACPlayerMovementComponent; }
	
	void DoubleJump();

	void Dash();

	void Die();

private:

	UPROPERTY( Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = ( AllowPrivateAccess = "true" ), DisplayName = "First Person Character Movement Component" )
	UPlayerMovementComponent* m_ACPlayerMovementComponent;

	UPROPERTY()
	UAbilityComponent* m_ACAbilityComponent;

	UPROPERTY( Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = ( AllowPrivateAccess = "true" ), DisplayName = "Ability Swap Position" )
	UAbility_PositionSwap* m_ACAbilitySwapPos;

	UPROPERTY( Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = ( AllowPrivateAccess = "true" ), DisplayName = "Ability Shield" )
	UAbility_Shield* m_ACAbilityShield;

	UPROPERTY( Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = ( AllowPrivateAccess = "true" ), DisplayName = "Melee Component" )
	UMeleeComponent* m_ACMeleeComponent;

	class UAIPerceptionStimuliSourceComponent* stimulus;

	void setUpStimulus();

	UPROPERTY()
	int m_iJumpCounter;

public: 

	UPROPERTY(EditAnywhere)
	float m_fJumpHeight;

	UPROPERTY(EditAnywhere)
	float m_fDashDistance;

	UPROPERTY(EditAnywhere)
	float m_fDashCooldown;

	UPROPERTY()
	bool m_bCanDash;

	bool GetPressedSprint();

	//int32 GetWeaponMaxReserveAmmo() { return m_iWeaponMaxReserveAmmo; }

	void SetPressedSprint(bool C) { m_bPressedSprint = C; }

	UPROPERTY()
	bool bPressedCrouch;

	UPROPERTY()
	bool m_bPressedSprint;

	UPROPERTY()
	bool m_bCanAttack;
};


