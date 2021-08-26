// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Module5Proj/Player/Components/AbilityComponent.h"
#include "Ability_Shield.generated.h"

class APlayerCharacter;

UCLASS()
class MODULE5PROJ_API UAbility_Shield : public UAbilityComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UAbility_Shield();

	void AbilityUsed();

	void ShieldDuration();

	void ShieldStop();

	bool GetShieldActive() const { return m_bAbilityActive; }

protected:

	APlayerCharacter* m_ACPlayerCharacter;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shield)
	UParticleSystem* m_pShieldParticleSystem;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shield)
	//UParticleSystemComponent* m_pShieldParticleComponent;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle TimerAbilityActive;

private:

	UPROPERTY(EditAnywhere)
	FVector m_v3EmitterSpawnOffset;

	UPROPERTY(EditAnywhere)
	float m_fMaxShieldDuration;

	UPROPERTY(EditAnywhere)
	float m_fCurrentShieldDuration;

};
