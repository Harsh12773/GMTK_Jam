#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Sidescroller_CameraManager.generated.h"

UCLASS()
class GMTK_JAM_API ASidescroller_CameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ASidescroller_CameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	float CameraDistance;

	UPROPERTY(EditAnywhere)
	float CameraHeight;

	UPROPERTY(EditAnywhere)
	float CameraLagSpeed;

	FVector InitialRightVector;
	bool bHasCachedRightVector = false;

};

