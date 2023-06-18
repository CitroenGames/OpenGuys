// CitroenGames 2023


#include "Core/CG_GameState.h"
#include "Net/UnrealNetwork.h"

void ACG_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACG_GameState, CountDownTimer);
}