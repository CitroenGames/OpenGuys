// CitroenGames 2023


#include "Core/CG_Gamemode.h"
#include "Core/CG_PlayerController.h"
#include "Core/CG_PlayerState.h"
#include "Core/CG_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

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

void ACG_Gamemode::SpawnCharacter(APlayerController* PlayerController, FName CheckPoint, int AppearanceID, bool IsMaleAppearance)
{
	// get controlled pawn
	APawn* ControlledPawn = PlayerController->GetPawn();
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
	}
	else
	{
		// spawn character at checkpoint location
		UE_LOG(LogTemp, Warning, TEXT("SpawnCharacter at checkpoint location"));
		// get all player start with tag
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), CheckPoint, PlayerStarts);
		// get random player start
		AActor* RandomPlayerStart = PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)];
		// spawn character at random player start
		FVector SpawnTransform = RandomPlayerStart->GetActorLocation();
		FRotator SpawnRotation = RandomPlayerStart->GetActorRotation();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// character class is CG_BaseCharacter
		UClass* CharacterClass = ACG_BaseCharacter::StaticClass();
		GetWorld()->SpawnActor<ACG_BaseCharacter>(CharacterClass, SpawnTransform, SpawnRotation, SpawnParameters);
	}
}

void ACG_Gamemode::SpawnSpectator(APlayerController* Player, bool SpawnAtPlayerLocation, FVector SpawnTransform)
{
	// spawn spectator
	UE_LOG(LogTemp, Warning, TEXT("SpawnSpectator"));
}