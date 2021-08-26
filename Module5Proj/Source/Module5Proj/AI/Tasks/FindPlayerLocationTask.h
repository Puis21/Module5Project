// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "FindPlayerLocationTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MODULE5PROJ_API UFindPlayerLocationTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UFindPlayerLocationTask(FObjectInitializer const& object_initiliazer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (ALlowProtectedAccess = "true"))
	bool search_random = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (ALlowProtectedAccess = "true"))
	float search_radius = 150.f;

};
