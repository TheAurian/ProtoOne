// All RIghts Reserved Copyright @ Whalefall Studios Inc

#pragma once

#include "Components/ActorComponent.h"
#include "SpikeDamage.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOONE_API USpikeDamage : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpikeDamage();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	//if spike component collides with a physicsbody, "hit" the actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard")
		class UStaticMeshComponent* SpikeComponent;

private:
	AActor* ActorTrigger;

	bool isActive;

	UPROPERTY(EditAnywhere, Category = "Hazard")
		ATriggerVolume* SpikeTrigger;

	UPROPERTY(EditAnywhere, Category = "Hazard")
		float SpikeDamageAmount = 20.f;
	/** Damaged the actor that triggered SpikeTrigger */
	void InflictDamage(AActor* ActorToDamage);
};
