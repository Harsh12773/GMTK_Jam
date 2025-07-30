#include "SideScroller/TrapPlatform.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"

ATrapPlatform::ATrapPlatform()
{
    PrimaryActorTick.bCanEverTick = true;

    // Platform setup
    PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
    RootComponent = PlatformMesh;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetBoxExtent(FVector(120.f, 120.f, 40.f));
    TriggerBox->SetRelativeLocation(FVector(0.f, 0.f, 30.f));

    // Spike setup (initially hidden and inactive)
    SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpikeMesh"));
    SpikeMesh->SetupAttachment(RootComponent);
    SpikeMesh->SetRelativeLocation(FVector(0.f, 0.f, -100.f)); // Start below the platform
    SpikeMesh->SetVisibility(false);
    SpikeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SpikeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SpikeCollision"));
    SpikeCollision->SetupAttachment(SpikeMesh);
    SpikeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disabled until spikes are up
}

void ATrapPlatform::BeginPlay()
{
    Super::BeginPlay();

    OriginalPlatformLocation = GetActorLocation();

    // Bind overlap events
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATrapPlatform::OnPlatformOverlapBegin);
    SpikeCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapPlatform::OnSpikeOverlapBegin);

    // --- Timeline Setup ---
    if (SpikeTimelineCurve)
    {
        FOnTimelineFloat TimelineUpdateCallback;
        FOnTimelineEvent TimelineFinishedCallback;

        TimelineUpdateCallback.BindUFunction(this, FName("UpdateSpikeAnimation"));
        TimelineFinishedCallback.BindUFunction(this, FName("FinishSpikeAnimation"));

        SpikeTimeline.AddInterpFloat(SpikeTimelineCurve, TimelineUpdateCallback);
        SpikeTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);

        // Store start/end locations for the spike animation
        SpikeStartLocation = SpikeMesh->GetRelativeLocation();
        SpikeEndLocation = FVector(SpikeStartLocation.X, SpikeStartLocation.Y, 0.f); // Animate to be flush with the platform
    }
}

void ATrapPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Advance the timeline every frame
    SpikeTimeline.TickTimeline(DeltaTime);

    // Apply shake effect if active
    if (bIsShaking)
    {
        const FVector ShakeOffset = FVector(FMath::RandRange(-ShakeIntensity, ShakeIntensity), FMath::RandRange(-ShakeIntensity, ShakeIntensity), 0.f);
        SetActorLocation(OriginalPlatformLocation + ShakeOffset);
    }
}

void ATrapPlatform::OnPlatformOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bHasBeenTriggered) return;

    if (OtherActor && OtherActor->ActorHasTag(FName("Player")) && OtherActor->GetActorLocation().Z > TriggerBox->GetComponentLocation().Z)
    {
        bHasBeenTriggered = true;
        GetWorld()->GetTimerManager().SetTimer(ActivateTimerHandle, this, &ATrapPlatform::ActivateTrap, TimeUntilActivate, false);
    }
}

void ATrapPlatform::ActivateTrap()
{
    bIsShaking = true;
    GetWorld()->GetTimerManager().SetTimer(ShakeTimerHandle, this, &ATrapPlatform::ExecuteTrapAction, ShakeDuration, false);
}

void ATrapPlatform::ExecuteTrapAction()
{
    bIsShaking = false;
    SetActorLocation(OriginalPlatformLocation); // Reset location before final action

    switch (TrapType)
    {
        case ETrapType::ETT_Breakable:
            // Optional: Spawn particles/sound for breaking effect
            Destroy();
            break;

        case ETrapType::ETT_Spikes:
            if (SpikeTimelineCurve)
            {
                SpikeMesh->SetVisibility(true);
                SpikeTimeline.PlayFromStart();
            }
            break;
    }
}

void ATrapPlatform::UpdateSpikeAnimation(float Value)
{
    // Lerp (Linear Interpolate) the spike mesh's location based on the curve's output value
    const FVector NewLocation = FMath::Lerp(SpikeStartLocation, SpikeEndLocation, Value);
    SpikeMesh->SetRelativeLocation(NewLocation);
}

void ATrapPlatform::FinishSpikeAnimation()
{
    // Once spikes are fully extended, enable their collision for damage
    SpikeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ATrapPlatform::OnSpikeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
    {
        // Apply damage to the overlapping actor
        UGameplayStatics::ApplyDamage(OtherActor, SpikeDamage, GetInstigatorController(), this, UDamageType::StaticClass());
    }
}