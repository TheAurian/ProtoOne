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
};



