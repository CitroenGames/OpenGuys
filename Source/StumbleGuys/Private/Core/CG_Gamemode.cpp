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

	//destroy controlled pawn
	if (ControlledPawn)
	{
		ControlledPawn->Destroy();
	}

	FVector SpawnTransform = FVector(0.f, 0.f, 0.f);
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	APlayerStart* RandomPlayerStart = nullptr;
	TArray<APlayerStart*> PlayerStarts;

	// create temporary array for player starts because GetAllActorsOfClass is a bish.
	TArray<AActor*> TempPlayerStarts;
	// get all actors of class and add to temp array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), TempPlayerStarts);
	// and then add to the player starts array
	for (AActor* TempActor : TempPlayerStarts)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(TempActor);
		if (PlayerStart)
		{
			PlayerStarts.Add(PlayerStart);
		}
	}

	// is checkpoint tag empty?
	if (CheckPoint == FName("None"))
	{
		// spawn character at default location
		UE_LOG(LogTemp, Warning, TEXT("Spawn Character at default location"));
		PlayerStarts.RemoveAll([CheckPoint](APlayerStart* PlayerStart) {
			return PlayerStart->PlayerStartTag.ToString() != CheckPoint.ToString();
		});
	}
	else
	{
		// spawn character at last checkpoint location
		UE_LOG(LogTemp, Warning, TEXT("Spawn Character at checkpoint location"));
		// filter player start with PlayerStart tag
		FString CheckPointString = CheckPoint.ToString();
		UE_LOG(LogTemp, Warning, TEXT("CheckPointString: %s"), *CheckPointString);
		PlayerStarts.RemoveAll([CheckPoint](APlayerStart* PlayerStart) {
			return PlayerStart->PlayerStartTag.ToString() != CheckPoint.ToString();
		});
	}

	//set spawn transform to the first player start location
	if (PlayerStarts.Num() > 0)
	{
		RandomPlayerStart = PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)];
		SpawnTransform = RandomPlayerStart->GetActorLocation();
		SpawnRotation = RandomPlayerStart->GetActorRotation();
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