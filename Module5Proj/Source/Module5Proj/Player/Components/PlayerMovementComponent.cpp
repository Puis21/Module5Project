// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UPlayerMovementComponent::UPlayerMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_fSprindSpeed = 1000.f;
}


// Called when the game starts
void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();


	
}


// Called every frame
void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Cyan, FString::SanitizeFloat(MaxWalkSpeed));
	}
}

void UPlayerMovementComponent::StartSprinting()
{
	MaxWalkSpeed = m_fSprindSpeed;
}

void UPlayerMovementComponent::StopSprinting()
{	
	MaxWalkSpeed = 600.f;
}

void UPlayerMovementComponent::StartCrouch()
{
	UE_LOG(LogTemp, Warning, TEXT("ALLO"));
	//bWantsToCrouch = true;
	//Crouch();
}