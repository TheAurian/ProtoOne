// Fill out your copyright notice in the Description page of Project Settings.

#include "ProtoOne.h"
#include "ProtoOneCamera.h"




AProtoOneCamera::AProtoOneCamera() {

	//set a default size for the sphere
	SphereSize = 300.0f;

	CameraSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CameraSphere"));
	CameraSphere->SetSphereRadius(SphereSize);

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

void AProtoOneCamera::SetNewTarget(ACharacter* NewTarget) {
	CameraTarget = NewTarget;
	//CameraSphere->SetupAttachment(CameraTarget->GetRootComponent());
	CameraSphere->AttachTo(CameraTarget->GetRootComponent());
	CameraSphere->SetVisibility(true, true);
}

