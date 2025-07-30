#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollectionComponent.generated.h"

class ACollectableActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GMTK_JAM_API UCollectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCollectionComponent();

	// Adds a specified quantity of an item to the inventory.
	UFUNCTION(BlueprintCallable, Category = "Collection")
	void AddItem(TSubclassOf<ACollectableActor> ItemClass, int32 Quantity = 1);

	// Removes a specified quantity of an item from the inventory.
	UFUNCTION(BlueprintCallable, Category = "Collection")
	void RemoveItem(TSubclassOf<ACollectableActor> ItemClass, int32 Quantity = 1);

	// Checks if the player has at least the required quantity of a specific item.
	UFUNCTION(BlueprintCallable, Category = "Collection")
	bool HasRequiredItem(TSubclassOf<ACollectableActor> ItemClass, int32 RequiredQuantity) const;

protected:
	// The inventory map. Key is the item class, Value is the quantity.
	// UPROPERTY makes sure it's managed by the garbage collector.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collection")
	TMap<TSubclassOf<ACollectableActor>, int32> Inventory;
};