// Fill out your copyright notice in the Description page of Project Settings.


#include "Shoot_Task.h"
#include "AIController.h"
#include "Module5Proj/AI/AI_Character.h"

UShoot_Task::UShoot_Task()
{
	NodeName = "AI_Shooting";
}

EBTNodeResult::Type UShoot_Task::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::ExecuteTask(owner_comp, node_memory);

	if (owner_comp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AAI_Character* AI_Char = Cast<AAI_Character>(owner_comp.GetAIOwner()->GetPawn());
	if (AI_Char == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AI_Char->Shoot();

	return EBTNodeResult::Succeeded;

}