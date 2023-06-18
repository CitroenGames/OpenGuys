// CitroenGames 2023


#include "Core/CG_Gamemode.h"
#include "Core/CG_PlayerController.h"
#include "Core/CG_PlayerState.h"
#include "Core/CG_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Core/CG_GameState.h"
#include "GameFramework/PlayerStart.h"

ACG_Gamemode::ACG_Gamemode()
{
	// set default pawn class to none
	DefaultPawnClass = nullptr;
	// set default player controller class to CG_PlayerController
	PlayerControllerClass = ACG_PlayerController::StaticClass();
	// set default player state class to CG_PlayerState
	PlayerStateClass = ACG_PlayerState::StaticClass();
	// set default game state class to CG_GameState
	GameStateClass = ACG_GameState::StaticClass();
}

void ACG_Gamemode::BeginPlay()
{
	Super::BeginPlay();
}

void ACG_Gamemode::SpawnCharacter(APlayerController* PlayerController, FName CheckPoint, int AppearanceID, bool IsMaleAppearance)
{
	// get controlled pawn
	APawn* ControlledPawn = PlayerController->GetPawn();
	FVector SpawnTransform;
	FRotator SpawnRotation;
	AActor* RandomPlayerStart;
	TArray<AActor*> PlayerStarts;

	//destroy controlled pawn
	if (ControlledPawn)
	{
		ControlledPawn->Destroy();
	}
	// is checkpoint tag empty?
	if (CheckPoint == "")
	{
		// spawn character at default location
		UE_LOG(LogTemp, Warning, TEXT("SpawnCharacter at default location"));
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
		// filter player start with PlayerStart tag
		for (int i = 0; i < PlayerStarts.Num(); i++)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[i]);
			if (PlayerStart != nullptr)
			{
				if (PlayerStart->PlayerStartTag != "None")
				{
					PlayerStarts.RemoveAt(i);
					// Decrement the counter as we have just removed an item from the TArray
					i--;
				}
			}
		}
	}
	else
	{
		// spawn character at Check Point location
		UE_LOG(LogTemp, Warning, TEXT("SpawnCharacter at default location"));
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
		// filter player start with PlayerStart tag
		for (int i = 0; i < PlayerStarts.Num(); i++)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[i]);
			if (PlayerStart != nullptr)
			{
				if (PlayerStart->PlayerStartTag != CheckPoint)
				{
					PlayerStarts.RemoveAt(i);
					// Decrement the counter as we have just removed an item from the TArray
					i--;
				}
			}
		}
	}
	// log all player start from array
	for (int i = 0; i < PlayerStarts.Num(); i++)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[i]);
		if (PlayerStart != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerStart: %s"), *PlayerStart->PlayerStartTag.ToString());
		}
	}
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// spawn character
	ACG_BaseCharacter* Character = GetWorld()->SpawnActor<ACG_BaseCharacter>(CharacterClass, SpawnTransform, SpawnRotation, SpawnParameters);
	// possess character
	PlayerController->Possess(Character);
	// set character appearance
	Character->ServerSetAppearance(IsMaleAppearance, AppearanceID);
	// set view target with blend
	PlayerController->SetViewTargetWithBlend(Character, 0.f, EViewTargetBlendFunction::VTBlend_Cubic);
}

void ACG_Gamemode::SpawnSpectator(APlayerController* Player, bool SpawnAtPlayerLocation, FVector SpawnTransform)
{
	// spawn spectator
	UE_LOG(LogTemp, Warning, TEXT("SpawnSpectator"));
}