#include "Component/CollectionComponent.h"

#include "SideScroller/CollectableActor.h"

UCollectionComponent::UCollectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCollectionComponent::AddItem(TSubclassOf<ACollectableActor> ItemClass, int32 Quantity)
{
	if (!ItemClass) return;

	// Find the item in the map. If it exists, add to its quantity. Otherwise, add a new entry.
	int32& Count = Inventory.FindOrAdd(ItemClass);
	Count += Quantity;
}

void UCollectionComponent::RemoveItem(TSubclassOf<ACollectableActor> ItemClass, int32 Quantity)
{
	if (!ItemClass) return;

	int32* FoundQuantity = Inventory.Find(ItemClass);
	if (FoundQuantity)
	{
		*FoundQuantity -= Quantity;
		// If quantity is zero or less, remove the item from the map.
		if (*FoundQuantity <= 0)
		{
			Inventory.Remove(ItemClass);
		}
	}
}

bool UCollectionComponent::HasRequiredItem(TSubclassOf<ACollectableActor> ItemClass, int32 RequiredQuantity) const
{
	if (!ItemClass) return false;

	const int32* FoundQuantity = Inventory.Find(ItemClass);
	if (FoundQuantity)
	{
		return *FoundQuantity >= RequiredQuantity;
	}

	return false;
}