// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/Actor.h"
#include "FuelCapsule.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogFuelCapsule, Log, All);

/**
 * Power-up fuel capsule.
 */
UCLASS()
class ZYNAPSRELOADED_API AFuelCapsule : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AFuelCapsule();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called when the fuel capsule hits other actor or an obstacle
	UFUNCTION(BlueprintNativeEvent, Category = ZynapsEvents)
	void Hit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult &HitResult);

	// Called when the fuel capsule begins overlapping with another actor
	UFUNCTION(BlueprintNativeEvent, Category = ZynapsEvents)
	void BeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult &SweepResult);

	// Called when the fuel capsule begins overlapping with another actor
	UFUNCTION(BlueprintNativeEvent, Category = ZynapsEvents)
	void EndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Collision capsule component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UCapsuleComponent* CapsuleComponent;

	// Static mesh component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* MeshComponent;

protected:

	// Checks that the projectile is within the viewport limits
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = Util)
	bool IsVisibleOnScreen() const;

private:

	// Creates the capsule component used for collision detection
	UCapsuleComponent* CreateCapsuleComponent(USceneComponent* Parent);

	// Creates the mesh component which models the fuel capsule
	UStaticMeshComponent* CreateMeshComponent(USceneComponent* Parent);

};
