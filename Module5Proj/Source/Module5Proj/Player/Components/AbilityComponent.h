// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MODULE5PROJ_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	float m_fMaxAbilitySize;

	UPROPERTY(VisibleAnywhere)
	float m_fCurrentAbilitySize;

	UPROPERTY(VisibleAnywhere)
	float m_fAbilityDecreaseRate;

	UPROPERTY(EditAnywhere)
	float m_fAbilityIncreaseRate;

	UPROPERTY(EditAnywhere)
	int m_iMaxAbilityCharges;

	UPROPERTY(VisibleAnywhere)
	int m_iCurrentAbilityCharges;

	UPROPERTY(VisibleAnywhere)
	bool m_bAbilityActive;

	UPROPERTY(VisibleAnywhere)
	bool m_bCanUseAbility;
		
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle TimerAbilityDecreaseRate;	
};
