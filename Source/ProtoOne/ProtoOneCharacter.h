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

	float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:


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

	/** Duration after taking damage where the player cannot take damage again */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float HitRecoveryDelay = 0.5f;

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
		float Damage_StabAmount = 30.f;

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

	/** This function initiates action events or evades (context-sensitive)*/
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void Action();

	/** This function initiates action events or evades (depending on game context)*/
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void AttackLeft();

	/** This function initiates action events or evades (depending on game context)*/
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void AttackRight();

	/** This function initiates action events or evades (depending on game context)*/
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void AttackStab();



	void InflictDamage(AActor* ActorToDamage);

private:

	FVector GetSweepRayTraceStart(EAttack SweepType);
	FVector GetSweepRayTraceEnd(EAttack SweepType);

	/** */
	const FHitResult GetSinglePhysicsBodyInRange(FVector LineTraceStart, FVector LineTraceEnd);

	//////////////////////////////////////////////////////////////////
	// DAMAGE AND HEALTH CONTROL SECTIONS

public:
	void AddHealth(float HealthToAdd);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetPlayerHealth() { return CurrentHealth; };

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetPlayerHealthPercentage() { return CurrentHealth / BaseHealth; };

	/** */
	void StartHitDelayTimer();

	UFUNCTION(Blueprintpure, Category = "Health")
	bool IsDead();

	void KillPlayer();
	void InflictDamage();
	void PlayerHittable();

	//not uproperty, because timerhandles cannot set as uproperties
	/** Timer to set when player can be hit after taking damage */
	FTimerHandle HittablePlayerTimer;
	FTimerHandle SynergySustainTimer;

	UFUNCTION(BlueprintCallable, category = "Synergy")
		void StartSynergySustainTimer();

	/** Adds a set amount of synergy to the player */
	UFUNCTION(BlueprintCallable, Category = "Synergy")
		void Synergize();

	/** Subtracts a set amount of synergy from the player */
	UFUNCTION(BlueprintCallable, Category = "Synergy")
		void DeSynergize(float Amount);
	
	/** Returns players current synergy level*/
	UFUNCTION(BlueprintPure, Category = "Synergy")
		float GetCurrentSynergy() { return CurrentSynergy; };

	/** Returns max possible synergy player can sustain */
	UFUNCTION(BlueprintPure, Category = "Synergy")
		float GetMaxSynergy() { return BaseSynergy; };

	/** Returns whether player recently gained synergy within the SynergizedPeriod*/
	UFUNCTION(BlueprintCallable, Category = "Synergy")
		bool WasRecentlySynergized() { return RecentlySynergized; };

	void StartSynergyDecay();

	// Will want a function that increases max synergization period, a function that increases max health, a function that synergizes player, and a function that gives player health


protected:
	/** 
	The base (or "FULL") health of the player. 
	The base health can be modified to increase the overall HP of the player
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float BaseHealth = 200.f;

	/** Current Health of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float CurrentHealth = BaseHealth;

	float HealthMultiplier = 1.f;
	bool CanBeHit = false;

	//Synergy
	
	///should be set in blueprints rather than hardcoded from here
	/**
	The base (or "FULL") synergy level of the player.
	The base synergy level can be modified to increase the overall synergy level of the player
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	float BaseSynergy = 100.f;				
	
	/** Current Synergy Level of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	float CurrentSynergy = BaseSynergy;
	
	///should be set in blueprints rather than hardcoded from here
	/** How long the player stays synergized without their synergy level decaying*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	float SynergizedPeriod = 2.f;  
	
	/** Returns whether the player has recently gained synergy */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Synergy")
	bool RecentlySynergized = false; 


};

