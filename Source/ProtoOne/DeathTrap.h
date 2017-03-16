// All RIghts Reserved Copyright @ Whalefall Studios Inc

#pragma once

#include "GameFramework/MovementComponent.h"
#include "DeathTrap.generated.h"

/**
 * 
 */
UCLASS()
class PROTOONE_API UDeathTrap : public UMovementComponent
{
	GENERATED_BODY()

	UDeathTrap();
	
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//rotates death trap
	void Move(float DeltaTime);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float RotationSpeed = 100.f;

};
