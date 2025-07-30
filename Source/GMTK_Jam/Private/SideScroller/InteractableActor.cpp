#include "SideScroller/InteractableActor.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Component/CollectionComponent.h"
#include "Kismet/GameplayStatics.h"

AInteractableActor::AInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    RootComponent = BaseMesh;

    InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionTrigger"));
    InteractionTrigger->SetupAttachment(RootComponent);

    // Bind the overlap function
    InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnInteractionOverlap);
}

void AInteractableActor::OnInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
    if (PlayerCharacter)
    {
        APlayerState* PlayerState = PlayerCharacter->GetPlayerState();
        if (PlayerState)
        {
            UCollectionComponent* CollectionComp = PlayerState->FindComponentByClass<UCollectionComponent>();
            if (CollectionComp && CollectionComp->HasRequiredItem(RequiredItemClass, RequiredItemQuantity))
            {
                // If the check is successful, fire the event
                OnInteractionSuccessful();
            }
        }
    }
}

// This is the default C++ implementation of the event.
// It can be left empty to be defined purely in Blueprint, or it can have base functionality.
void AInteractableActor::OnInteractionSuccessful_Implementation()
{
    // For example, you could remove the items from the player's inventory here.
    if (ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        if (APlayerState* PlayerState = PlayerCharacter->GetPlayerState())
        {
            if (UCollectionComponent* CollectionComp = PlayerState->FindComponentByClass<UCollectionComponent>())
            {
                CollectionComp->RemoveItem(RequiredItemClass, RequiredItemQuantity);
            }
        }
    }

    // And then destroy the interactable actor.
    Destroy();
}