// Fill out your copyright notice in the Description page of Project Settings.

#include "ProtoOne.h"
#include "ProtoOneCameraComponent.h"


// Sets default values for this component's properties
UProtoOneCameraComponent::UProtoOneCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;



	CameraSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CameraSphere"));
	// ...

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	PlayerBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerBoom"));
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));

	//set a default size for the sphere
	SphereSize = 300.0f;

	CameraBoomDistance = 600.0f;
	PlayerBoomDistance = 200.0f;
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

	//connect the
	CameraSphere->AttachTo(PlayerBoom);
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

