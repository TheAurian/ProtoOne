// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "ProtoOneGameMode.generated.h"

UCLASS(minimalapi)
class AProtoOneGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProtoOneGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class AProtoOneCamera* MainCamera;

	virtual void BeginPlay() override;

protected:

	/** HUD Attributes*/

	//UE4 macro included for garbage collection only (need no other keywords)
	UPROPERTY()
	class UUserWidget* CurrentWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "HUD", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;
};



