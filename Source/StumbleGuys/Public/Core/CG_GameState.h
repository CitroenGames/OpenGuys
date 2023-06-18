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
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CountDownTimer)
		int CountDownTimer;
	
	ACG_GameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	// blueprint enheritable
	UFUNCTION(BlueprintImplementableEvent, Category = "Count Down")
		void RoundStart();

private:
	UFUNCTION()
		void StartCountDown();

	UFUNCTION()
		void OnRep_CountDownTimer();

	FTimerHandle CountDownTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* TimerEndSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* TimerTickSound;
};
