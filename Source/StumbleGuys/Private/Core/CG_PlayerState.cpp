// CitroenGames 2023


#include "Core/CG_PlayerState.h"
#include "Net/UnrealNetwork.h"

ACG_PlayerState::ACG_PlayerState()
{
	// set player state replicated
	SetReplicates(true);

}

int ACG_PlayerState::GetColorID()
{
	if (AppearanceIsInit)
	return AppearanceColorID;
	else
	{
		InitAppearance();
		// hope that it will be initialized
		UE_LOG(LogTemp, Warning, TEXT("ACG_PlayerState::GetColorID() AppearanceIsInit is false!"));
		return AppearanceColorID;
	}
}

void ACG_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	// FIXME: THIS DOESNT SEEM TO REPLICATED WELL
	InitAppearance();
}
void ACG_PlayerState::InitAppearance_Implementation()
{
	// random bool for bAppearanceMale
	bAppearanceMale = FMath::RandBool();
	// random int for AppearanceColorID
	AppearanceColorID = FMath::RandRange(0, 7);
	// set AppearanceIsInit to true because we have initialized the appearance
	AppearanceIsInit = true;
}

void ACG_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// call the parent function
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// replicate the AppearanceIsInit variable
	DOREPLIFETIME(ACG_PlayerState, AppearanceIsInit);
	// replicate the CurrentCheckPoint variable
	DOREPLIFETIME(ACG_PlayerState, CurrentCheckPoint);
	// replicate the AppearanceColorID variable
	DOREPLIFETIME(ACG_PlayerState, AppearanceColorID);
	// replicate the bAppearanceMale variable
	DOREPLIFETIME(ACG_PlayerState, bAppearanceMale);
}

void ACG_PlayerState::ServerSetCheckPoint_Implementation(FName CheckPoint)
{
	CurrentCheckPoint = CheckPoint;
	UE_LOG(LogTemp, Warning, TEXT("Player %s has reached checkpoint %s"), *GetPlayerName(), *CheckPoint.ToString());
}