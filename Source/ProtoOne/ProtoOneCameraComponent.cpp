// Fill out your copyright notice in the Description page of Project Settings.

#include "ProtoOne.h"
#include "ProtoOneCameraComponent.h"


// Sets default values for this component's properties
UProtoOneCameraComponent::UProtoOneCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//set a default size for the sphere
	SphereSize = 300.0f;

	CameraSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CameraSphere"));
	// ...

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));


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


// Called when the game starts
void UProtoOneCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UProtoOneCameraComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UProtoOneCameraComponent::SetNewTarget(ACharacter* NewTarget) {
	CameraTarget = NewTarget;
	//CameraSphere->SetupAttachment(CameraTarget->GetRootComponent());
	CameraSphere->AttachTo(CameraTarget->GetRootComponent());
	CameraSphere->SetSphereRadius(SphereSize);
	CameraSphere->SetVisibility(true, true);

	CameraBoom->SetupAttachment(CameraTarget->GetRootComponent());
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller


	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PlayerCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm */
}

