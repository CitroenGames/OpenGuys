// CitroenGames 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CG_GameState.generated.h"

/**
 * 
 */
UCLASS()
class STUMBLEGUYS_API ACG_GameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Replicated)
		int CountDownTimer = 15;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
