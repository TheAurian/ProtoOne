// All RIghts Reserved Copyright @ Whalefall Studios Inc

#include "ProtoOne.h"
#include "EnemyAICharacter.h"


// Sets default values
AEnemyAICharacter::AEnemyAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("Spawned Enemy's current health is: %s"), *FString::SanitizeFloat(CurrentHealth));


}

// Called when the game starts or when spawned
void AEnemyAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AEnemyAICharacter::MainAttack() {
	return;
}

float AEnemyAICharacter::TakeDamage(
	float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {

	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//if enemy is not dead
	if (!IsDead()) {
		//damage enemy
		CurrentHealth -= ActualDamage;
		UE_LOG(LogTemp, Warning, TEXT("Enemy Took this much damage from %s: %s HP"), *DamageCauser->GetName() ,*FString::SanitizeFloat(ActualDamage));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Enemy is already dead"));
		return 0.f;
	}

	UE_LOG(LogTemp, Warning, TEXT("Enemy's remaining health: %s HP"), *FString::SanitizeFloat(CurrentHealth));

	if (CurrentHealth <= 0) {
		UE_LOG(LogTemp, Error, TEXT("The actor that killed this enemy was: %s"), *DamageCauser->GetName());
		//this->GetCharacterMovement()->Velocity
		Kill();
	}

	return -1;
}

bool AEnemyAICharacter::IsDead() {
	return CurrentHealth <= 0;
}

void AEnemyAICharacter::Kill() {
	UE_LOG(LogTemp, Warning, TEXT("Enemy has lost ALL health and is now dead!: %s HP"), *FString::SanitizeFloat(CurrentHealth));
	GetCapsuleComponent()->DestroyComponent();
	GetMesh()->SetSimulatePhysics(true);
	// sound/visual effect
	// may need to call Destroy() and GetWorld()->ForceGarbageCollection(true)
}

