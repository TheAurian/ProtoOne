// All RIghts Reserved Copyright @ Whalefall Studios Inc

#include "ProtoOne.h"
#include "EnemyAICharacter.h"
#include "ProtoOneCharacter.h"


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
	UE_LOG(LogTemp, Warning, TEXT("%s is attacking!"), *this->GetName());
	FVector LineTraceEnd = GetActorLocation() + this->GetActorRotation().Vector() * DebugAttackLineLength;

	auto HitResult = GetSinglePhysicsBodyInRange(GetActorLocation(),  LineTraceEnd);
	auto HitActor = HitResult.GetActor();

	//if we hit an actor and that actor is the player (avoid hits on Tierra or other enemies)
	AProtoOneCharacter* PlayerCharacter = Cast<AProtoOneCharacter>(HitActor);
	if (PlayerCharacter) {
		UE_LOG(LogTemp, Warning, TEXT("Stab Damaged Target: %s"), *HitActor->GetName());
		InflictDamage(PlayerCharacter);
	}
	else {
		if (HitActor) {
			UE_LOG(LogTemp, Warning, TEXT("Error - Undetected Stab of Target: %s"), *HitActor->GetName());
		}
	}
	// get actor that hit, if any and inflict damage


	return;
}

const FHitResult AEnemyAICharacter::GetSinglePhysicsBodyInRange(FVector LineTraceStart, FVector LineTraceEnd) {

	/// setup query parameters (always ignore yourself byt getting owner or the first hit we get will be ourself)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);

	///
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		LineTraceStart,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
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

void AEnemyAICharacter::InflictDamage(AActor* ActorToDamage) {
	//cast to a character and call take damage
	ACharacter* DamagedActor = Cast<ACharacter>(ActorToDamage);
	if (DamagedActor) {
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		DamagedActor->TakeDamage(Damage_Amount, DamageEvent, this->GetController(), this);
	}

	//include sound effect
	//include visual effect
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

