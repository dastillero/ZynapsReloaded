// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/Pawn.h"
#include "ZynapsPlayerState.h"
#include "Projector2DComponent.h"
#include "PlayerProjectile.h"
#include "PlayerPawn.generated.h"

// This constants are applied to calculate collisions with the screen margins
const float LimitMarginUp = 7.0f;
const float LimitMarginDown = 3.0f;
const float LimitMarginLeft = 5.0f;
const float LimitMarginRight = 9.0f;

// Constants to identify the three cannons of the ship
const uint8 RightCannon = 0;
const uint8 LeftCannon = 1;
const uint8 TopCannon = 2;

/**
 * The default Pawn class used by StageGameMode while playing the game.
 */
UCLASS()
class ZYNAPSRELOADED_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Returns a reference to the instance of AZynapsPlayerState or NULL if it doesn't exist
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	AZynapsPlayerState* GetZynapsPlayerState() const;

	// Called to move the player up
	UFUNCTION(BlueprintCallable, Category = ZynapsActions)
	void MoveUp(float Val);

	// Called to move the player down
	UFUNCTION(BlueprintCallable, Category = ZynapsActions)
	void MoveDown(float Val);

	// Called to move the player to the left
	UFUNCTION(BlueprintCallable, Category = ZynapsActions)
	void MoveLeft(float Val);

	// Called to move the player to the right
	UFUNCTION(BlueprintCallable, Category = ZynapsActions)
	void MoveRight(float Val);

	// Called to fire
	UFUNCTION(BlueprintCallable, Category = ZynapsActions)
	void Fire();

	// Called when a player hits other actor
	UFUNCTION(BlueprintNativeEvent, Category = ZynapsEvents)
	void Hit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult &HitResult);

	// Called when the player begins overlapping with another actor
	UFUNCTION(BlueprintNativeEvent, Category = ZynapsEvents)
	void BeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult &SweepResult);

	// Called when the player begins overlapping with another actor
	UFUNCTION(BlueprintNativeEvent, Category = ZynapsEvents)
	void EndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Collision capsule
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UCapsuleComponent* CapsuleComponent;

	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* MeshComponent;

	// Engine particle system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UParticleSystemComponent* EnginePartSystemComponent;

	// Projector2DComponent to make transformations to screen coordinates
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UProjector2DComponent* Projector2DComponent;

	// The player's movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actor)
	float MovementSpeed;

	// The type of projectile spawned when firing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	TSubclassOf<class APlayerProjectile> ProjectileClass;

	// The name of the socket for the left cannon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	FName LeftCannonSocketName;

	// The name of the socket for the right cannon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	FName RightCannonSocketName;

	// The name of the socket for the top cannon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	FName TopCannonSocketName;

	// The name of the socket where the engine thrust particle system will be attached
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	FName EngineThrustSocketName;

protected:

	// Returns the transform of a socket
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	FTransform GetSocketTransform(FName SocketName) const;

private: 

	// Viewport size update each tick
	FVector2D ViewportSize;

	// Scene view used to convert from world coordinates to screen coordinates
	FSceneView* SceneView;

	// The next cannon to be shot
	uint8 NextCannon;
};
