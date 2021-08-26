// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeComponent.h"
#include "Module5Proj/Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UMeleeComponent::UMeleeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeleeComponent::BeginPlay()
{
	Super::BeginPlay();

    m_ACPlayerCharacter = Cast<APlayerCharacter>(GetOwner());

}


// Called every frame
void UMeleeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMeleeComponent::ActivateCollision()
{
	if(m_ACPlayerCharacter)
	{
		m_ACPlayerCharacter->CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UMeleeComponent::DeactivateCollision()
{
	if(m_ACPlayerCharacter)
	{
		m_ACPlayerCharacter->CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_ACPlayerCharacter->m_bCanAttack = true;
	}
}