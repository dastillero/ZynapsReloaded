// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/Actor.h"
#include "PlayerProjectile.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogPlayerProjectile, Log, All);

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
	virtual void Tick(float DeltaSeconds) override;

protected:

	// Checks that the projectile is within the viewport limits
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = Util)
	bool IsVisibleOnScreen() const;

};
