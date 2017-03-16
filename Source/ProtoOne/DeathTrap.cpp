// All RIghts Reserved Copyright @ Whalefall Studios Inc

#include "ProtoOne.h"
#include "DeathTrap.h"


UDeathTrap::UDeathTrap() {

	PrimaryComponentTick.bCanEverTick = true;
}

void UDeathTrap::BeginPlay() {
	Super::BeginPlay();

}

void UDeathTrap::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move(DeltaTime);
}

void UDeathTrap::Move(float DeltaTime) {
	FRotator NewRotation = GetOwner()->GetActorRotation().Add(0.f, 100.f*DeltaTime, 0.f);

	GetOwner()->SetActorRotation(NewRotation);
}