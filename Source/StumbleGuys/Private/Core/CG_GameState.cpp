// CitroenGames 2023

#include "Core/CG_GameState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

ACG_GameState::ACG_GameState()
{
	// set default values
	CountDownTimer = 15;
}

void ACG_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACG_GameState, CountDownTimer);
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
	if (CountDownTimer > 0)
	{
		// play sound effect 2d
		UGameplayStatics::PlaySound2D(GetWorld(), TimerTickSound);
		GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &ACG_GameState::StartCountDown, 1.0f, false);
	}
	else
	{
		RoundStart();
		// play sound effect 2d
		UGameplayStatics::PlaySound2D(GetWorld(), TimerEndSound);
	}
}