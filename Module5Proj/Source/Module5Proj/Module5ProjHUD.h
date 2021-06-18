// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Module5ProjHUD.generated.h"

UCLASS()
class AModule5ProjHUD : public AHUD
{
	GENERATED_BODY()

public:
	AModule5ProjHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

