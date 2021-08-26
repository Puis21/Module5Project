// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h" //del
#include "Module5Proj/AI/AI_Controller.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Module5Proj/AI/BlackBoard_Keys.h"

UFindPlayerLocationTask::UFindPlayerLocationTask(FObjectInitializer const& object_initiliazer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UFindPlayerLocationTask::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	//get player char and the npc's controller
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const cont = Cast<AAI_Controller>(owner_comp.GetAIOwner());

	//get player location
	FVector const player_location = player->GetActorLocation();
	if (search_random)
	{
		FNavLocation loc;

		//get the nav syst and generate a rand loc near the player
		UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (nav_sys->GetRandomPointInNavigableRadius(player_location, search_radius, loc, nullptr))
		{
			cont->getBlackboard()->SetValueAsVector(bb_keys::target_location, loc.Location);
		}
	}
	else
	{
		cont->getBlackboard()->SetValueAsVector(bb_keys::target_location, player_location);

	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}