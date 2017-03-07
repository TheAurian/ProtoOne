// Fill out your copyright notice in the Description page of Project Settings.

#include "ProtoOne.h"
#include "ProtoOneCamera.h"




AProtoOneCamera::AProtoOneCamera() {
	CameraSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CameraSphere"));
	// ...

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	PlayerBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerBoom"));
	//PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));

	//set a default size for the sphere
	SphereSize = 300.0f;
	CameraBoomDistance = 600.0f;
	PlayerBoomDistance = 200.0f;
	RecenterDelay = 1.0f;

	/*
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PlayerCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm */
}

void AProtoOneCamera::BeginPlay() {
	Super::BeginPlay();

	ACharacter* target = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (target) {
		//CameraTarget = UGameplayStatics::GetPlayerCharacter(this, 0);
		SetNewTarget(target);
	}
}

void AProtoOneCamera::SetNewTarget(ACharacter* NewTarget) {
	CameraTarget = NewTarget;
	//CameraSphere->SetupAttachment(CameraTarget->GetRootComponent());

	//connect the
	CameraSphere->AttachTo(CameraTarget->GetRootComponent());
	CameraSphere->SetSphereRadius(SphereSize);
	CameraSphere->SetVisibility(true, true);
	CameraSphere->SetHiddenInGame(false);

	PlayerBoom->SetupAttachment(CameraTarget->GetRootComponent());
	PlayerBoom->TargetArmLength = PlayerBoomDistance;
	PlayerBoom->bUsePawnControlRotation = false;

	CameraBoom->SetupAttachment(CameraSphere);
	CameraBoom->TargetArmLength = CameraBoomDistance; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PlayerCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm */

	
}

