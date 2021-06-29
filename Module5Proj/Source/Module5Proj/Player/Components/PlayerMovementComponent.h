// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"


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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
