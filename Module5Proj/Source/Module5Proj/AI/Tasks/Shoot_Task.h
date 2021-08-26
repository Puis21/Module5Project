// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Shoot_Task.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MODULE5PROJ_API UShoot_Task : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UShoot_Task();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

};
