// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivateMeleeDamage.h"
#include "Module5Proj/Player/Components/MeleeComponent.h"

void UActivateMeleeDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (nullptr != MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();
		if (nullptr != Owner)
		{
			UMeleeComponent* m_pAMeleeComp = nullptr;
            m_pAMeleeComp = Cast<UMeleeComponent>(Owner->GetComponentByClass(UMeleeComponent::StaticClass()));
            if (nullptr != m_pAMeleeComp)
            {
                m_pAMeleeComp->ActivateCollision();;
            }
        }
    }
}