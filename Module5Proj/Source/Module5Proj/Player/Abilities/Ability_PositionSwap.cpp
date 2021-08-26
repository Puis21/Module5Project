// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_PositionSwap.h"
#include "Kismet/GameplayStatics.h"
#include "Module5Proj/AI/AI_Character.h"
#include "Module5Proj/Player/PlayerCharacter.h"
#include "Module5Proj/Player/Components/Camera/PlayerCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

UAbility_PositionSwap::UAbility_PositionSwap()
{
    m_fMaxAbilitySize = 30.f;
    m_fCurrentAbilitySize = m_fMaxAbilitySize;
    m_fAbilityDecreaseRate = 0.25f;
    m_bCanUseAbility = true;
}

void UAbility_PositionSwap::BeginPlay()
{
    Super::BeginPlay();

    m_ACPlayerCharacter = Cast<APlayerCharacter>(GetOwner());

}

void UAbility_PositionSwap::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Purple, FString::SanitizeFloat(m_fCurrentAbilitySize));
	}

}

void UAbility_PositionSwap::AbilityActivated()
{
    if(m_bCanUseAbility)
    {
        GetWorld()->GetTimerManager().SetTimer(TimerAbilityDecreaseRate, this, &UAbility_PositionSwap::DecreaseAbilitySize, m_fAbilityDecreaseRate, true);
        m_ACPlayerCharacter->GetFirstPersonCameraComponent()->PostProcessSettings.ColorGamma = FVector4(1.350000, 1.040000, 1.060000, 0.600000);
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25);  
        m_bAbilityActive = true;
    }

}

void UAbility_PositionSwap::AbilityCancelled()
{
    GetWorld()->GetTimerManager().ClearTimer(TimerAbilityDecreaseRate);
    m_ACPlayerCharacter->GetFirstPersonCameraComponent()->PostProcessSettings.ColorGamma = FVector4(1, 1, 1, 1);
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
    m_bAbilityActive = false;
}

void UAbility_PositionSwap::AbilityUsed()
{
    if(m_ACPlayerCharacter)
    {
        
    FVector PlayerLoc;
	FRotator PlayerRot;
	FHitResult Hit;

	m_ACPlayerCharacter->GetController()->GetPlayerViewPoint(PlayerLoc, PlayerRot);

	FVector Start = PlayerLoc;
	FVector End = Start + (PlayerRot.Vector() * 2000);

	FCollisionQueryParams TraceParams;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	if(AAI_Character* HitMarkerCast = Cast<AAI_Character>(Hit.GetActor()))
	{
		FVector EnemyPos = HitMarkerCast->GetActorLocation();

		HitMarkerCast->SetActorLocation(PlayerLoc);
		m_ACPlayerCharacter->TeleportTo(EnemyPos, FRotator::ZeroRotator, false, false);
        m_ACPlayerCharacter->AddControllerYawInput(m_ACPlayerCharacter->GetActorRotation().Yaw + 360.f);
        AbilityCancelled();
	}

	if(bHit)
	DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5,5,5), FColor::Emerald, false, 2.0f);

    //m_ACPlayerCharacter->FirstPersonCameraComponent.PostProcessSettings.ColorGamma = FVector4(1, 1, 1, 1);
    }

}

void UAbility_PositionSwap::DecreaseAbilitySize()
{

    if(m_fCurrentAbilitySize <= m_fMaxAbilitySize && m_fCurrentAbilitySize != 0)
    {  
        m_fCurrentAbilitySize--;
    }
    else
    {
        m_bCanUseAbility = false;
    }

    if(m_fCurrentAbilitySize == 0)
    {
        AbilityCancelled();
    }
  
}