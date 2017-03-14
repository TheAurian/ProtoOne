// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "ProtoOneCharacter.generated.h"

// Enum to define different attacks
UENUM(BlueprintType)
enum class EAttack
{
	AT_LEFT			UMETA(DisplayName="Left Sweep"),
	AT_RIGHT		UMETA(DisplayName = "Right Sweep"),
	AT_STAB			UMETA(DisplayName = "Stab")

};

UCLASS(config=Game)
class AProtoOneCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Arm connecting the character to the Usphere*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* PlayerBoom;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Main camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* PlayerCamera;

	/** Sphere that that camera uses determine when to follow */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CameraSphere;

public:
	AProtoOneCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:



	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** The size of the sphere around the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float SphereSize;

	/** The time the camera will wait before recentering on the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float RecenterDelay;

	/** The distance or length of boom between the character and the center of the CameraSphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float PlayerBoomDistance;

	/** The distance or length of boom between the camera and the center of the CameraSphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraBoomDistance;

	/** Camera lag*/
	float CameraLag = 1.f;

	/** Duration after being damaged and cannot move */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float HitStunDelay = 0.5f;

	/** Duration after attacking and cannot move */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRecoveryDelay = 0.5f;

	/** Debug Attack/Spear Trace Length */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float DebugAttackLineLength = 80.f;

	/** How far from PC's center does sweep attack start */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float SweepLineOffset = 50.f;

	/** Sweep attack angle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float DebugAttackAngle = 35.f;

	/** Damage value for stab attaks */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float Damage_Stab = 30.f;

	/** Damage value for sweep attaks */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float Damage_Sweep = 45.f;


public:
	/** Returns PlayerBoom object*/
	FORCEINLINE USpringArmComponent* GetPlayerBoom() const { return PlayerBoom; };

	/** Returns CameraBoom object*/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; };

	/** Returns PlayerCamera object*/
	FORCEINLINE UCameraComponent* GetPlayerCamera() const { return PlayerCamera; };

	/** Returns CameraSphere object*/
	FORCEINLINE USphereComponent* GetCameraSphere() const { return CameraSphere; };

	/*
		SECTION FOR PLAYER CONTROL FUNCTIONS
		- each attack function should include an audio que, an animation reference and a visual effect
	*/

	/** This function initiates action events or evades (depending on game context)*/
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void AttackLeft();
	/** This function initiates action events or evades (depending on game context)*/
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void AttackRight();

	/** This function initiates action events or evades (depending on game context)*/
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void AttackStab();

	/** This function initiates action events or evades (depending on game context)*/
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void Action();

private:

	FVector GetSweepRayTraceStart(EAttack SweepType);

	FVector GetSweepRayTraceEnd(EAttack SweepType);

	/** */
	const FHitResult GetSinglePhysicsBodyInRange(FVector LineTraceStart, FVector LineTraceEnd);

};

