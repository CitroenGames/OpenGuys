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
	AActor* RandomPlayerStart = nullptr;
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
		UE_LOG(LogTemp, Warning, TEXT("Spawn Character at default location"));
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
		// filter player start with PlayerStart tag
		for (int i = 0; i < PlayerStarts.Num(); i++)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[i]);
			if (PlayerStart != nullptr)
			{
				FString PlayerStartTag = PlayerStart->PlayerStartTag.ToString();
				if (!PlayerStartTag.Equals("Start", ESearchCase::IgnoreCase))
				{
					UE_LOG(LogTemp, Warning, TEXT("PlayerStartTag: %s"), *PlayerStart->PlayerStartTag.ToString());
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
		UE_LOG(LogTemp, Warning, TEXT("Spawn Character at Current CheckPoint location"));
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
	//set spawn transform to the first player start location
	if (PlayerStarts.Num() > 0)
	{
		RandomPlayerStart = PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)];
		SpawnTransform = RandomPlayerStart->GetActorLocation();
		SpawnRotation = RandomPlayerStart->GetActorRotation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerStarts found"));
		SpawnTransform = FVector(0.f, 0.f, 0.f);
		SpawnRotation = FRotator(0.f, 0.f, 0.f);
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// spawn character
	ACG_BaseCharacter* Character = GetWorld()->SpawnActor<ACG_BaseCharacter>(CharacterClass, SpawnTransform, SpawnRotation, SpawnParameters);
	// possess character
	PlayerController->Possess(Character);
	PlayerController->SetViewTargetWithBlend(Character, 0.f, EViewTargetBlendFunction::VTBlend_Cubic);
	Character->ServerSetAppearance(IsMaleAppearance, AppearanceID);
}

void ACG_Gamemode::SpawnSpectator(APlayerController* Player, bool SpawnAtPlayerLocation, FVector SpawnTransform)
{
	// spawn spectator
	UE_LOG(LogTemp, Warning, TEXT("SpawnSpectator"));
}