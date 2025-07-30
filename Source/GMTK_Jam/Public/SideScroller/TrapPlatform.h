#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h" // Required for Timelines
#include "TrapPlatform.generated.h"

// Enum to define the behavior of the trap. BlueprintType makes it usable in Blueprints.
UENUM(BlueprintType)
enum class ETrapType : uint8
{
    ETT_Breakable   UMETA(DisplayName = "Breakable"),
    ETT_Spikes      UMETA(DisplayName = "Spikes")
};

class UBoxComponent;
class UCurveFloat;

UCLASS()
class GMTK_JAM_API ATrapPlatform : public AActor
{
    GENERATED_BODY()

public:
    ATrapPlatform();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // --- COMPONENTS ---

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* PlatformMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SpikeMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* SpikeCollision;

    // --- SETTINGS ---

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Settings")
    ETrapType TrapType = ETrapType::ETT_Breakable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Settings")
    float TimeUntilActivate = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Settings")
    float ShakeDuration = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Settings")
    float ShakeIntensity = 5.0f;

    // --- SPIKE SETTINGS ---

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Settings|Spikes")
    float SpikeDamage = 25.f;

    // A curve asset to control the spike animation speed and feel
    UPROPERTY(EditAnywhere, Category = "Trap Settings|Spikes")
    UCurveFloat* SpikeTimelineCurve;

private:
    // --- OVERLAP FUNCTIONS ---
    UFUNCTION()
    void OnPlatformOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSpikeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // --- TRAP LOGIC ---
    void ActivateTrap();
    void ExecuteTrapAction();

    // --- SPIKE ANIMATION (TIMELINE) ---
    FTimeline SpikeTimeline;
    FVector SpikeStartLocation;
    FVector SpikeEndLocation;

    UFUNCTION()
    void UpdateSpikeAnimation(float Value);

    UFUNCTION()
    void FinishSpikeAnimation();

    // --- STATE & HELPERS ---
    FTimerHandle ActivateTimerHandle;
    FTimerHandle ShakeTimerHandle;
    FVector OriginalPlatformLocation;
    bool bIsShaking = false;
    bool bHasBeenTriggered = false;
};