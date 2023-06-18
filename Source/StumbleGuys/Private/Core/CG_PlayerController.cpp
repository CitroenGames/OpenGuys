// CitroenGames 2023


#include "Core/CG_PlayerController.h"
#include "Core/CG_PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Core/CG_Gamemode.h"
#include "Core/CG_GameState.h"


ACG_PlayerController::ACG_PlayerController()
{
	// cast to CG_PlayerState from player state
	PlayerStateRefrence = Cast<ACG_PlayerState>(PlayerState);
}

void ACG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	// set input mode game only 
	SetInputMode(FInputModeGameOnly());
	InitPlayerController();
}

void ACG_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACG_PlayerController, IsPlaying);
}

void ACG_PlayerController::ServerSpawnCharacter_Implementation()
{
	IsPlaying = true;
	// spawn character from gamemode
	ACG_Gamemode* Gamemode = Cast<ACG_Gamemode>(GetWorld()->GetAuthGameMode());
	if (Gamemode)
	{
		// get appearance id and ismaleappearance from player state
		const int AppearanceID = PlayerStateRefrence->GetColorID();
		const bool IsMaleAppearance = PlayerStateRefrence->GetMaleAppearance();
		// get current checkpoint from player state
		const FName CheckPoint = PlayerStateRefrence->GetCurrentCheckPoint();
		Gamemode->SpawnCharacter(this, CheckPoint, AppearanceID, IsMaleAppearance);
	}
}

void ACG_PlayerController::ServerSpawnSpectator_Implementation(bool SpawnAtPlayerLocation)
{
	IsPlaying = false;
	// spawn spectator from gamemode
	ACG_Gamemode* Gamemode = Cast<ACG_Gamemode>(GetWorld()->GetAuthGameMode());
	if (Gamemode)
	{
		// get controlled pawn
		APawn* ControlledPawn = GetPawn();
		// get location of controlled pawn
		FVector SpawnTransform = ControlledPawn->GetActorLocation();
		Gamemode->SpawnSpectator(this, SpawnAtPlayerLocation, SpawnTransform);
	}
}

void ACG_PlayerController::InitPlayerController()
{
	// log 
	UE_LOG(LogTemp, Warning, TEXT("InitPlayerController"));
	// if locally controlled
	if (IsLocalController())
	{
		// if countdown isnt 0 spawn character from GameState
		ACG_GameState* GameState = Cast<ACG_GameState>(GetWorld()->GetGameState());
		if (GameState)
		{
			if (GameState->CountDownTimer != 0)
			{
				ServerSpawnCharacter();
				// log
				UE_LOG(LogTemp, Warning, TEXT("Spawned Character"));
			}
			else
			{
				ServerSpawnSpectator(false);
				// log
				UE_LOG(LogTemp, Warning, TEXT("Spawned Spectator"));
			}
		}
	}
}