#include "GameFramework/SideScroller_CameraManager.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"

ASidescroller_CameraManager::ASidescroller_CameraManager(): InitialRightVector()
{
	CameraDistance = 1000.0f;
	CameraHeight = 200.0f;
	CameraLagSpeed = 2.0f;
}

void ASidescroller_CameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	APawn* TargetPawn = GetOwningPlayerController()->GetPawn();
	if (!TargetPawn)
		return;

	const FVector TargetLocation = TargetPawn->GetActorLocation();

	if (!bHasCachedRightVector)
	{
		InitialRightVector = -TargetPawn->GetActorRightVector();
		bHasCachedRightVector = true;
	}

	FVector DesiredLocation = TargetLocation 
		- (InitialRightVector * CameraDistance) 
		+ FVector(0.0f, 0.0f, CameraHeight);   

	FVector SmoothedLocation = UKismetMathLibrary::VInterpTo(
		GetCameraLocation(),
		DesiredLocation,
		DeltaTime,
		CameraLagSpeed
	);

	FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(
		SmoothedLocation,
		TargetLocation + FVector(0.0f, 0.0f, CameraHeight * 0.5f)
	);

	OutVT.POV.Location = SmoothedLocation;
	OutVT.POV.Rotation = CameraRotation;
}
