// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "ProtoOneCameraComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOONE_API UProtoOneCameraComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProtoOneCameraComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

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

	UFUNCTION(BlueprintCallable, Category = Camera)
		void SetNewTarget(ACharacter* NewTarget);

	/** Returns PlayerBoom object*/
	FORCEINLINE USpringArmComponent* GetPlayerBoom() const { return PlayerBoom; };

	/** Returns CameraBoom object*/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; };

	/** Returns PlayerCamera object*/
	FORCEINLINE UCameraComponent* GetPlayerCamera() const { return PlayerCamera; };

	/** Returns CameraSphere object*/
	FORCEINLINE USphereComponent* GetCameraSphere() const { return CameraSphere; };


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		ACharacter* CameraTarget;

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

private:
	
};
