// CitroenGames 2023


#include "Core/CG_Gamemode.h"
#include "Core/CG_PlayerController.h"
#include "Core/CG_PlayerState.h"

ACG_Gamemode::ACG_Gamemode()
{
	// set default pawn class to none
	DefaultPawnClass = nullptr;
	// set default player controller class to CG_PlayerController
	PlayerControllerClass = ACG_PlayerController::StaticClass();
	// set default player state class to CG_PlayerState
	PlayerStateClass = ACG_PlayerState::StaticClass();
}

void ACG_Gamemode::BeginPlay()
{
	Super::BeginPlay();
}

void ACG_Gamemode::SpawnCharacter(int AppearanceID, bool IsMaleAppearance)
{
	// spawn character
	UE_LOG(LogTemp, Warning, TEXT("SpawnCharacter"));
}

void ACG_Gamemode::SpawnSpectator(APlayerController* Player, bool SpawnAtPlayerLocation, FVector SpawnTransform)
{
	// spawn spectator
	UE_LOG(LogTemp, Warning, TEXT("SpawnSpectator"));
}