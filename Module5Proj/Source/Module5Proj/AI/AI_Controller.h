// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Module5Proj/Player/PlayerCharacter.h"
#include "Perception/AIPerceptionTypes.h"
#include "AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class MODULE5PROJ_API AAI_Controller : public AAIController
{
	GENERATED_BODY()
	
public:

	AAI_Controller(FObjectInitializer const& object_initialiazer = FObjectInitializer::Get());
	
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void OnPossess(APawn* const pawn) override;
	class UBlackboardComponent* getBlackboard() const;

	APlayerCharacter* GetPlayerCharacter() const;

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviortreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* Behaviortree;

	class UBlackboardComponent* BlacboardComponent;

	class UAISenseConfig_Sight* sightConfig;

	UFUNCTION()
	void onUpdated(TArray<AActor*> const& updated_actors);

	void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);

	void SetUpPeceptionSystem();

};
