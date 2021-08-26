// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Module5Proj/Player/Components/AbilityComponent.h"
#include "Ability_PositionSwap.generated.h"

class APlayerCharacter;

UCLASS()
class MODULE5PROJ_API UAbility_PositionSwap : public UAbilityComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UAbility_PositionSwap();

 	void AbilityActivated();
	
	void AbilityCancelled();

	void AbilityUsed();

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle Timer;	

	bool GetAbilityActive() const { return m_bAbilityActive; }

protected:
 
	UPROPERTY()
	APlayerCharacter* m_ACPlayerCharacter;

private:

	UFUNCTION()
	void DecreaseAbilitySize();

};
