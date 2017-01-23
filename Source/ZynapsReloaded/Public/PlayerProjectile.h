// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/Actor.h"
#include "Projector2DComponent.h"
#include "PlayerProjectile.generated.h"

UCLASS()
class ZYNAPSRELOADED_API APlayerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Projector2DComponent to make transformations to screen coordinates
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UProjector2DComponent* Projector2DComponent;
	
protected:
	// Checks that the projectile is within the viewport limits
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = Util)
	bool IsVisibleOnScreen() const;

};
