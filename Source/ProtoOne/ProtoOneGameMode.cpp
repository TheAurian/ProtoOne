// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ProtoOne.h"
#include "ProtoOneGameMode.h"
#include "ProtoOneCharacter.h"
#include "ProtoOneCamera.h"
#include "Blueprint/UserWidget.h"


AProtoOneGameMode::AProtoOneGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;


	}



}

void AProtoOneGameMode::BeginPlay(){
	Super::BeginPlay();

	// create HUD
	if (HUDWidgetClass) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget) {
			CurrentWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("HUD created successfully!"))
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("CurrentWidget failed to be created.!"))
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("HUDWidgetClass failed to be created (is a nullptr?)!"))
	}

	/*WIP camera setup
	MainCamera = CreateDefaultSubobject<AProtoOneCamera>(TEXT("MainCamera"));
	if (MainCamera != nullptr) {
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (PlayerCharacter != nullptr) {
			MainCamera->SetNewTarget(PlayerCharacter);
			UE_LOG(LogTemp, Warning, TEXT("Main camera created successfully."));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Unable to create MainCamera. Player character pointer is null."));
		}

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Unable to create MainCamera. It's pointer is null."));
	}*/
}
