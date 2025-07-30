#include "SideScroller/CollectableActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"
#include "Component/CollectionComponent.h"

ACollectableActor::ACollectableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetSphereRadius(100.f);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Mesh doesn't need collision

	// Bind the overlap function
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACollectableActor::OnOverlapBegin);
}

void ACollectableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Try to cast the overlapping actor to a Character
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// Get the PlayerState from the Character
		APlayerState* PlayerState = PlayerCharacter->GetPlayerState();
		if (PlayerState)
		{
			// Find the CollectionComponent on the PlayerState
			UCollectionComponent* CollectionComp = PlayerState->FindComponentByClass<UCollectionComponent>();
			if (CollectionComp)
			{
				// Add this item's class to the collection and then destroy this actor
				CollectionComp->AddItem(this->GetClass());
				Destroy();
			}
		}
	}
}