// CitroenGames 2023

#include "Core/CG_GameState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Core/CG_PlayerController.h"
#include "Core/CG_Gamemode.h"

ACG_GameState::ACG_GameState()
{
	// set default values
	CountDownTimer = 15;
}

void ACG_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACG_GameState, CountDownTimer);
	DOREPLIFETIME(ACG_GameState, RoundEnded);
	DOREPLIFETIME(ACG_GameState, WinnerRef);
}

void ACG_GameState::BeginPlay()
{
	Super::BeginPlay();
	// has auth
	if (HasAuthority())
	{
		// start countdown
		StartCountDown();
	}
}

void ACG_GameState::StartCountDown()
{
	CountDownTimer--;
	OnRep_CountDownTimer();
}

void ACG_GameState::OnRep_CountDownTimer()
{
	if (CountDownTimer == 2)
	{
		ACG_PlayerController* CGPlayerController = Cast<class ACG_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (CGPlayerController)
		{
			// play match start
			CGPlayerController->PlayMatchReady();
		}
	}
	if (CountDownTimer != 0)
	{
		// play sound effect 2d
		UGameplayStatics::PlaySound2D(GetWorld(), TimerTickSound);
		// this isnt super accurate but it works
		GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &ACG_GameState::StartCountDown, 1.0f, false);
	}
	else
	{
		RoundStart();
		// play sound effect 2d
		UGameplayStatics::PlaySound2D(GetWorld(), TimerEndSound);
	}
}

void ACG_GameState::OnRep_RoundEnded()
{
	if (RoundEnded)
	{
		// play sound effect 2d
		UGameplayStatics::PlaySound2D(GetWorld(), GameEndSound);
		// get cg player controller
		ACG_PlayerController* CGPlayerController = Cast<class ACG_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (CGPlayerController)
		{
			// play match Over
			CGPlayerController->PlayMatchOver();
		}

	}
}

void ACG_GameState::OnRep_WinnerRef()
{
	// has auth
	if (HasAuthority())
	{
		RoundEnded = true;
		OnRep_RoundEnded();
	}
}

void ACG_GameState::RoundStart()
{
	// Cast to game mode and unlock player movement
	ACG_Gamemode* CGGameMode = Cast<ACG_Gamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CGGameMode)
	{
		CGGameMode->EnableCharacterMovement();
	}
}
