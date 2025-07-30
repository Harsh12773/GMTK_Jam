#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SideScroller_SaveGame.generated.h"

UCLASS()
class GMTK_JAM_API USideScroller_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// The name of the level the player was on.
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FString LevelName;

	// The player's location in the level.
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FVector PlayerLocation;

	// Example: Player's health.
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float PlayerHealth;

	// Default constructor
	USideScroller_SaveGame();
};
