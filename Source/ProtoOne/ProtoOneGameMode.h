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
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class AProtoOneCamera* MainCamera;

	UFUNCTION(BlueprintPure, category = "HUD")
		FString GetHUDHealth() { return HUD_Health; };

	UFUNCTION(BlueprintPure, category = "HUD")
		FString GetHUDSynergy() { return HUD_Synergy; };

	class AProtoOneCharacter* PlayerCharacter;

	virtual void BeginPlay() override;

protected:

	/** HUD Attributes*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "HUD", meta = (BlueprintProtected = "true"))
		FString HUD_Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "HUD", meta = (BlueprintProtected = "true"))
		FString HUD_Synergy;

	//UE4 macro included for garbage collection only (need no other keywords)
	UPROPERTY()
	class UUserWidget* CurrentWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "HUD", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** Game Mode determined Player valuyes*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Synergy", meta = (BlueprintProtected = "true"))
		float SynergyDecayRate;



};



