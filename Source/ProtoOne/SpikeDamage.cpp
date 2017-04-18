// All RIghts Reserved Copyright @ Whalefall Studios Inc

#include "ProtoOne.h"
#include "SpikeDamage.h"
#include "ProtoOneCharacter.h"


// Sets default values for this component's properties
USpikeDamage::USpikeDamage()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	isActive = true;
	// ...
}


// Called when the game starts
void USpikeDamage::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Death Spike initializing..."));

	// ...

	ActorTrigger = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (ActorTrigger) {
		UE_LOG(LogTemp, Warning, TEXT("Death Spike Actor Trigger %s!"), *ActorTrigger->GetName());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Unable to initialize Actor Trigger "), *GetOwner()->GetName());
	}
	
}


// Called every frame
void USpikeDamage::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// If comes in contact with Player Character, damage them
	if (SpikeTrigger->IsOverlappingActor(ActorTrigger)) {
		//UE_LOG(LogTemp, Warning, TEXT("Actor hit by spike!"));
		InflictDamage(ActorTrigger);
	}

}

void USpikeDamage::InflictDamage(AActor* ActorToDamage) {
	//cast to our player character and call take damage
	AProtoOneCharacter* PlayerCharacter = Cast<AProtoOneCharacter>(ActorToDamage);
	if (PlayerCharacter) {

		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		PlayerCharacter->TakeDamage(SpikeDamageAmount, DamageEvent, UGameplayStatics::GetPlayerController(this, 0), PlayerCharacter);
	}
	//include sound effect
	//include visual effect
}
