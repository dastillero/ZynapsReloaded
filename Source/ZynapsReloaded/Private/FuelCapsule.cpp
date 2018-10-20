// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "FuelCapsule.h"
#include "ProjectionUtil.h"

// Log category
DEFINE_LOG_CATEGORY(LogFuelCapsule);

// Sets default values
AFuelCapsule::AFuelCapsule() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Set up the collision capsule
	CapsuleComponent = CreateCapsuleComponent(RootComponent);

	// Set up the mesh component
	MeshComponent = CreateMeshComponent(CapsuleComponent);
}

// Creates the capsule component used for collision detection
UCapsuleComponent* AFuelCapsule::CreateCapsuleComponent(USceneComponent* Parent)
{
	UCapsuleComponent* Component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	Component->InitCapsuleSize(45.0, 70.0f);
	Component->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
	Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Component->SetGenerateOverlapEvents(true);
	Component->SetNotifyRigidBodyCollision(true);
	Component->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Component->SetCanEverAffectNavigation(false);
	Component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Component->SetSimulatePhysics(true);
	Component->SetEnableGravity(false);
	Component->SetAngularDamping(0.0f);
	Component->SetLinearDamping(0.0f);
	Component->BodyInstance.bLockXRotation = true;
	Component->BodyInstance.bLockZRotation = true;
	Component->BodyInstance.bLockXTranslation = true;
	//Component->SetHiddenInGame(false);
	Component->WakeRigidBody();
	Component->OnComponentBeginOverlap.AddDynamic(this, &AFuelCapsule::BeginOverlap);
	Component->OnComponentEndOverlap.AddDynamic(this, &AFuelCapsule::EndOverlap);
	Component->OnComponentHit.AddDynamic(this, &AFuelCapsule::Hit);
	Component->SetupAttachment(Parent);

	return Component;
}

// Creates the mesh component which models the ship
UStaticMeshComponent* AFuelCapsule::CreateMeshComponent(USceneComponent* Parent)
{
	UStaticMeshComponent* Component = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(
		TEXT("StaticMesh'/Game/Models/FuelCapsule/FuelCapsule.FuelCapsule'"));
	if (MeshObj.Succeeded())
	{
		Component->SetStaticMesh(MeshObj.Object);
		Component->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
		Component->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Component->SetSimulatePhysics(false);
		Component->SetupAttachment(Parent);
	}
	else
	{
		UE_LOG(LogFuelCapsule, Error,
			TEXT("The asset StaticMesh'/Game/Models/FuelCapsule/FuelCapsule.FuelCapsule'"));
		Component = nullptr;
	}

	return Component;
}

// Called when the game starts or when spawned
void AFuelCapsule::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFuelCapsule::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Destroy the fuel capsule if it is not visible anymore
	if (!IsVisibleOnScreen())
	{
		Destroy();
	}
}

// Checks that the projectile is within the viewport limits
bool AFuelCapsule::IsVisibleOnScreen() const
{
	// Get the player controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogFuelCapsule, Error, TEXT("Failed to retrieve the player controller"));
		return false;
	}

	// Check that the fuel capsule is within screen limits
	AActor* Actor = (AActor*)this;
	FVector2D Position = UProjectionUtil::GetLocationInScreenCoordinates(PlayerController, Actor);
	FVector2D Size = UProjectionUtil::GetSizeInScreenCoordinates(PlayerController, Actor);
	FVector2D ViewportSize = UProjectionUtil::GetViewportSize(PlayerController);
	if (Position.X + Size.X / 2 < 0 || Position.X - Size.X / 2 > ViewportSize.X ||
		Position.Y + Size.Y / 2 < 0 || Position.Y + Size.Y / 2 > ViewportSize.Y)
	{
		return false;
	}
	return true;
}

// Called when the fuel capsule hits other actor or an obstacle
void AFuelCapsule::Hit_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &HitResult)
{
	//PRINT("AFuelCapsule::Hit");
}

// Called when the fuel capsule begins overlapping with another actor
void AFuelCapsule::BeginOverlap_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult &SweepResult)
{
	//PRINT("AFuelCapsule::BeginOverlap");
}

// Called when the fuel capsule ends overlapping with another actor
void AFuelCapsule::EndOverlap_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//PRINT("AFuelCapsule::EndOverlap");
}

