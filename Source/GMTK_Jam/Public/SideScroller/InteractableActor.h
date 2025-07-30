#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

class UBoxComponent;
class ACollectableActor;

UCLASS()
class GMTK_JAM_API AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractableActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* InteractionTrigger;

	// The class of item required to interact
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TSubclassOf<ACollectableActor> RequiredItemClass;

	// The quantity of the item required
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	int32 RequiredItemQuantity = 1;

	// Called when an actor overlaps the trigger
	UFUNCTION()
	void OnInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Event that fires in Blueprint when the interaction is successful
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnInteractionSuccessful();
	virtual void OnInteractionSuccessful_Implementation();
};