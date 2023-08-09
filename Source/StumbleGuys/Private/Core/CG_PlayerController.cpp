// CitroenGames 2023


#include "Core/CG_PlayerController.h"
#include "Core/CG_PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Core/CG_Gamemode.h"
#include "Core/CG_GameState.h"
#include "Kismet/GameplayStatics.h"

void ACG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	// set input mode game only 
	SetInputMode(FInputModeGameOnly());
	// cast to CG_PlayerState from player state
	PlayerStateRefrence = Cast<ACG_PlayerState>(PlayerState);
	InitPlayerController();
}

void ACG_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACG_PlayerController, IsPlaying);
}

void ACG_PlayerController::ServerSpawnCharacter_Implementation()
{
	if (!PlayerStateRefrence)
	{
		UE_LOG(LogTemp, Error, TEXT("ACG_PlayerController::ServerSpawnCharacter_Implementation PlayerStateRefrence is null!"));
		return;
	}

	ACG_Gamemode* Gamemode = Cast<ACG_Gamemode>(GetWorld()->GetAuthGameMode());
	if (!Gamemode)
		return;

	// Retrieve player state values
	const int AppearanceID = PlayerStateRefrence->GetColorID();
	const bool IsMaleAppearance = PlayerStateRefrence->GetMaleAppearance();
	const FName CheckPoint = PlayerStateRefrence->GetCurrentCheckPoint();

	if (!CheckPoint.IsNone())
	{
		Gamemode->SpawnCharacter(this, CheckPoint, 1, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint is empty"));
		Gamemode->SpawnCharacter(this, "", 1, true);
	}

	IsPlaying = true;
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

// end play
void ACG_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// if endplay is because of removed from world or quit game then we leave and return to main menu
	if (EndPlayReason == EEndPlayReason::Quit || EndPlayReason == EEndPlayReason::RemovedFromWorld)
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
		if (OnlineSub)
		{
			IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
			if (Sessions.IsValid())
			{
				Sessions->DestroySession(NAME_GameSession);
			}
		}

		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			UGameplayStatics::OpenLevel(World, "MainMenu");
		}
	}
}

void ACG_PlayerController::PlayMatchOver_Implementation()
{
	PlayMatchOverBP();
}

void ACG_PlayerController::PlayMatchReady_Implementation()
{
	PlayMatchReadyBP();
}