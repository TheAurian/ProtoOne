// All RIghts Reserved Copyright @ Whalefall Studios Inc

#pragma once

#include "GameFramework/Character.h"
#include "EnemyAICharacter.generated.h"

UCLASS()
class PROTOONE_API AEnemyAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAICharacter();

	/** Function for the enemy taking damage */
	float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/*
	SECTION FOR ENEMY ACTIONS
	- each action function should include an audio que, an animation reference and a visual effect
	*/

	UFUNCTION(BlueprintCallable, Category = "Attack")
		void MainAttack();
	
	/** Enemy attributes */

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealth() { return CurrentHealth; };

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealthPercentage() { return CurrentHealth / BaseHealth; };

	bool IsDead();
	void Kill();
	//void InflictDamage();
	void IsHittable();

protected:
	// Health 
	/**
	The base (or "FULL") health of the enemy.
	The base health can be modified to increase the overall HP of the enemy (ie. for different enemy classes
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		float BaseHealth = 100.f;

	/** Current Health of the enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float CurrentHealth = BaseHealth;
};
