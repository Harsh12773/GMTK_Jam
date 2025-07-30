#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectableActor.generated.h"

class USphereComponent;

UCLASS()
class GMTK_JAM_API ACollectableActor : public AActor
{
	GENERATED_BODY()

public:
	ACollectableActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionSphere;

	// Called when another actor overlaps with the collision sphere
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};