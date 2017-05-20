// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ProtoOne.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProtoOneCharacter.h"
#include "ProtoOneCameraComponent.h"

#define OUT


//////////////////////////////////////////////////////////////////////////
// AProtoOneCharacter

AProtoOneCharacter::AProtoOneCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;

	////
	//// CAMERA SETUP
	////

	CameraSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CameraSphere"));
	// ...

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	PlayerBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerBoom"));
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));

	//set a default size for the sphere
	SphereSize = 1000.0f;
	CameraBoomDistance = 600.0f;
	PlayerBoomDistance = 200.0f;
	RecenterDelay = 1.0f;

	//connect the
	CameraSphere->AttachTo(RootComponent);
	CameraSphere->SetSphereRadius(SphereSize);
	CameraSphere->SetHiddenInGame(false);

	PlayerBoom->SetupAttachment(CameraSphere);
	PlayerBoom->TargetArmLength = PlayerBoomDistance;
	PlayerBoom->bUsePawnControlRotation = false;

	CameraBoom->SetupAttachment(CameraSphere);
	CameraBoom->TargetArmLength = CameraBoomDistance; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PlayerCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm */

	///synergize player at spawn
	Synergize();

	/// wait after player spawns before they can take damage
	StartHitDelayTimer();

	UE_LOG(LogTemp, Warning, TEXT("Player Character's current health is: %s"), *FString::SanitizeFloat(CurrentHealth));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}



//////////////////////////////////////////////////////////////////////////
// Input

void AProtoOneCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AProtoOneCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProtoOneCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProtoOneCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProtoOneCharacter::LookUpAtRate);

	//handle player attacking bindings
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AProtoOneCharacter::Action);
	PlayerInputComponent->BindAction("Sweep Left", IE_Pressed, this, &AProtoOneCharacter::AttackLeft);
	PlayerInputComponent->BindAction("Sweep Right", IE_Pressed, this, &AProtoOneCharacter::AttackRight);
	PlayerInputComponent->BindAction("Stab", IE_Pressed, this, &AProtoOneCharacter::AttackStab);
}

void AProtoOneCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProtoOneCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProtoOneCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AProtoOneCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

////////////////////////////////////////////////////////////////////////////////
// ATTACKING 

void AProtoOneCharacter::Action() {
	UE_LOG(LogTemp, Warning, TEXT("ACTION BUTTON PRESSED"));
}

void AProtoOneCharacter::AttackRight() {
	UE_LOG(LogTemp, Warning, TEXT("ATTACK/SWEEP RIGHT BUTTON PRESSED"));

	auto HitResult = GetSinglePhysicsBodyInRange(GetSweepRayTraceStart(EAttack::AT_RIGHT), GetSweepRayTraceEnd(EAttack::AT_RIGHT));
	auto HitActor = HitResult.GetActor();
	if (HitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Right Sweep Damaged  Target: %s"), *HitActor->GetName());

		///call actors TakeDamage function
		InflictDamage(HitActor);

	}
}

void AProtoOneCharacter::AttackLeft() {
	UE_LOG(LogTemp, Warning, TEXT("ATTACK/SWEEP LEFT BUTTON PRESSED"));

	auto HitResult = GetSinglePhysicsBodyInRange(GetSweepRayTraceStart(EAttack::AT_LEFT), GetSweepRayTraceEnd(EAttack::AT_LEFT));
	auto HitActor = HitResult.GetActor();
	if (HitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Left Sweep Damaged Target: %s"), *HitActor->GetName());

		///call actors TakeDamage function
		InflictDamage(HitActor);
	}

}


void AProtoOneCharacter::AttackStab() {
	UE_LOG(LogTemp, Warning, TEXT("ATTACK/STAB BUTTON PRESSED"));
	
	FVector LineTraceEnd = GetActorLocation() + GetActorRotation().Vector() * DebugAttackLineLength;

	auto HitResult = GetSinglePhysicsBodyInRange(GetActorLocation(), LineTraceEnd);
	auto HitActor = HitResult.GetActor();
	if (HitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Stab Damaged Target: %s"), *HitActor->GetName());

		InflictDamage(HitActor);
	}

	///FOR DEBUGGING, CAN BE REMOVED
	Synergize();
}

void AProtoOneCharacter::InflictDamage(AActor* ActorToDamage) {
	//cast to our player character and call take damage
	ACharacter* DamagedActor = Cast<ACharacter>(ActorToDamage);
	if (DamagedActor) {

		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		DamagedActor->TakeDamage(Damage_StabAmount, DamageEvent, UGameplayStatics::GetPlayerController(this, 0), this);
	}
	//include sound effect
	//include visual effect
}

FVector AProtoOneCharacter::GetSweepRayTraceStart(EAttack SweepType) {

	// determine direction and rotation sweep traces in
	float direction = 1;
	if (SweepType == EAttack::AT_LEFT) {
		direction *= -1;
	}

	//move line trace off to the left of our PC
	FRotator offset = GetActorRotation().Add(0.f, 90.f*direction, 0.f);
	return GetActorLocation() + offset.Vector() * SweepLineOffset;
}

FVector AProtoOneCharacter::GetSweepRayTraceEnd(EAttack SweepType) {

	// determine direction and rotation sweep traces in
	float direction = 1;
	if (SweepType == EAttack::AT_LEFT) {
		direction *= -1;
	}

	// end the trace in a diagonal line 45 degrees right
	FRotator SweepEnd = GetActorRotation() + FRotator(0.f, DebugAttackAngle*direction, 0.f);
	return GetSweepRayTraceStart(SweepType) + SweepEnd.Vector() * DebugAttackLineLength;
}

const FHitResult AProtoOneCharacter::GetSinglePhysicsBodyInRange(FVector LineTraceStart, FVector LineTraceEnd) {

	/// setup query parameters (always ignore yourself byt getting owner or the first hit we get will be ourself)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, UGameplayStatics::GetPlayerCharacter(this, 0));

	///
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult, 
		LineTraceStart,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	for (int i = 0; i < 1; i++) {
		DrawDebugLine(
			GetWorld(),
			LineTraceStart,
			LineTraceEnd,
			FColor(255, 0, 0),
			true,
			1.f,
			0,
			10.f
		);
	}
	return HitResult;
}

/////////////////////////////////////////////////
//// Player Stats Management Functions

void AProtoOneCharacter::AddHealth(float HealthToAdd) {

}

bool AProtoOneCharacter::IsDead() {
	return CurrentHealth <= 0;
}

void AProtoOneCharacter::KillPlayer() {
	UE_LOG(LogTemp, Warning, TEXT("Player has lost ALL health and is now dead!: %s HP"), *FString::SanitizeFloat(CurrentHealth));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController) {
		PlayerController->SetCinematicMode(true, false, false, true, true);
		GetMesh()->SetSimulatePhysics(true);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("UNABLE to kill player. Could not retrieve player controller!"));
	}
	// sound/visual effect
	// run game over UI
	// initiate game restart
	// may need to call Destroy() and GetWorld()->ForceGarbageCollection(true)
}

void AProtoOneCharacter::PlayerHittable() {
	CanBeHit = true;
}

void AProtoOneCharacter::StartHitDelayTimer() {
	if (GetWorld()) {
		GetWorldTimerManager().SetTimer(HittablePlayerTimer, this, &AProtoOneCharacter::PlayerHittable, HitRecoveryDelay, false);
	}
}

float AProtoOneCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// if player is not dead
	if (!IsDead() & CanBeHit) {
		// damage player
		CurrentHealth -= ActualDamage;
		UE_LOG(LogTemp, Warning, TEXT("Player Took this much damage: %s HP"), *FString::SanitizeFloat(ActualDamage));
		CanBeHit = false;
		StartHitDelayTimer();
	}
	else {
		//player can't be damaged at this time, so leave
		return 0.f;
	}

	UE_LOG(LogTemp, Warning, TEXT("Player's remaining health: %s HP"), *FString::SanitizeFloat(CurrentHealth));

	// if player is now dead
	if (CurrentHealth <= 0) {
		// kill player
		KillPlayer();
	}

	
	return ActualDamage;
}

void AProtoOneCharacter::InflictDamage() {

}

/** SYNERGIZE FUNCTIONS */
void AProtoOneCharacter::Synergize() {
	StartSynergySustainTimer();
	RecentlySynergized = true;
}

void AProtoOneCharacter::DeSynergize(float Amount) {
	if (CurrentSynergy - Amount > 0) {
		CurrentSynergy -= Amount;
		//UE_LOG(LogTemp, Warning, TEXT("Current synergy has been decreased"));
	}
}

void AProtoOneCharacter::StartSynergySustainTimer() {
	if (GetWorld()) {
		GetWorldTimerManager().SetTimer(SynergySustainTimer, this, &AProtoOneCharacter::StartSynergyDecay, SynergizedPeriod, false);
	}
}

void AProtoOneCharacter::StartSynergyDecay() {
	RecentlySynergized = false;
}