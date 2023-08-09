// CitroenGames 2023

#include "Core/CG_Gamemode.h"
#include "Core/CG_PlayerController.h"
#include "Core/CG_PlayerState.h"
#include "Core/CG_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Core/CG_GameState.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	RefreshPlayerStarts();
}

void ACG_Gamemode::PostLogin(APlayerController* NewPlayer)
{
	AllPlayerControllers.Add(NewPlayer);
}

void ACG_Gamemode::Logout(AController* Exiting)
{
	AllPlayerControllers.Remove(Cast<APlayerController>(Exiting));
}

void ACG_Gamemode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	AllPlayerControllers.Add(NewPC);
}

void ACG_Gamemode::SpawnCharacter(APlayerController* PlayerController, FName CheckPoint, int AppearanceID, bool IsMaleAppearance)
{
	// get controlled pawn
	APawn* ControlledPawn = PlayerController->GetPawn();

	//destroy controlled pawn
	if (ControlledPawn)
	{
		ControlledPawn->Destroy();
	}

	// if playerstart array is empty, refresh playerstart array
	if (PlayerStarts.IsEmpty())
	{
		RefreshPlayerStarts();
	}

	FVector SpawnTransform = FVector(0.f, 0.f, 0.f);
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	APlayerStart* RandomPlayerStart = nullptr;

	// filter player start with PlayerStart tag
	FString CheckPointString = CheckPoint.ToString();
	UE_LOG(LogTemp, Warning, TEXT("CheckPointString: %s"), *CheckPointString);

	TArray<APlayerStart*> LocalPlayerStart = PlayerStarts;
	// is checkpoint tag empty?
	if (CheckPoint == FName("None"))
	{
		// spawn character at default location
		UE_LOG(LogTemp, Warning, TEXT("Spawn Character at default location"));
		LocalPlayerStart.RemoveAll([CheckPoint](APlayerStart* PlayerStart) {
			return PlayerStart->PlayerStartTag != CheckPoint;
		});
	}
	else
	{
		// spawn character at last checkpoint location
		UE_LOG(LogTemp, Warning, TEXT("Spawn Character at checkpoint location"));
		LocalPlayerStart.RemoveAll([CheckPoint](APlayerStart* PlayerStart) {
			return PlayerStart->PlayerStartTag != CheckPoint;
		});
	}

	//set spawn transform to the first player start location
	if (LocalPlayerStart.Num() > 0)
	{
		RandomPlayerStart = LocalPlayerStart[FMath::RandRange(0, (LocalPlayerStart.Num() - 1))];
		SpawnTransform = RandomPlayerStart->GetActorLocation();
		SpawnRotation = RandomPlayerStart->GetActorRotation();
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// spawn character
	ACG_BaseCharacter* Character = GetWorld()->SpawnActor<ACG_BaseCharacter>(CharacterClass, SpawnTransform, SpawnRotation, SpawnParameters);
	// possess character
	PlayerController->Possess(Character);
	PlayerController->SetViewTargetWithBlend(Character, 0.f, EViewTargetBlendFunction::VTBlend_Cubic);
	Character->ServerSetAppearance(IsMaleAppearance, AppearanceID);

	// Cast MyGameState and check if the countdown timer is not 0; if it is, lock the player movement by setting movement mode to none
	ACG_GameState* CGGameState = Cast<ACG_GameState>(UGameplayStatics::GetGameState(this));
	if (CGGameState)
	{
		if (CGGameState->GetCountDownTimer() > 0)
		{
			// We need to get the movement component and set it as a local variable
			UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
			if (CharacterMovementComponent)
			{
				CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_None);
			}
		}
	}
}

void ACG_Gamemode::SpawnSpectator(APlayerController* Player, bool SpawnAtPlayerLocation, FVector SpawnTransform)
{
	// TODO spawn spectator
	UE_LOG(LogTemp, Warning, TEXT("SpawnSpectator"));
}

void ACG_Gamemode::RefreshPlayerStarts()
{
	PlayerStarts.Empty();
	PlayerStarts.Reserve(200);
	for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APlayerStart* PlayerStart = *ActorItr;
		if (PlayerStart)
		{
			PlayerStarts.Add(PlayerStart);
		}
	}
	PlayerStarts.Shrink();
}

void ACG_Gamemode::EnableCharacterMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("EnableCharacterMovement"));
	// TODO enable character movement by iterating through all player controllers
	// and getting the controlled pawn and setting movement mode to walking
	
	for (TActorIterator<ACG_BaseCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ACG_BaseCharacter* Character = *ActorItr;
		if (Character)
		{
			// We need to get the movement component and set it as a local variable
			UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
			if (CharacterMovementComponent)
			{
				CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
			}
		}
	}
	
	// also do a sphere trace by channel visibility with a 100 as a radius
	// and call onplayercollision with all of the actors hit
}

void ACG_Gamemode::TraveltoLevel()
{
	// get current level name and switch depending on level name
	FString CurrentLevelName = GetWorld()->GetMapName();
	UE_LOG(LogTemp, Warning, TEXT("CurrentLevelName: %s"), *CurrentLevelName);
	switch (*CurrentLevelName)
	{
	case "Gameplay_Map1_Level":
		UGameplayStatics::OpenLevel(GetWorld(), "Level_02");
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("ERROR: MAP IS NOT VALID"));
		break;
	}
}
