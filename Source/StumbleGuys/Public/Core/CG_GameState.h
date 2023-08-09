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
	
	ACG_GameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	// blueprint enheritable
	UFUNCTION()
		void RoundStart();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RoundEnded)
		bool RoundEnded;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_WinnerRef)
		class APlayerState* WinnerRef;

	UFUNCTION(BlueprintCallable)
		void OnRep_WinnerRef();

	UFUNCTION()
		int GetCountDownTimer() const { return CountDownTimer; }

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, ReplicatedUsing = OnRep_CountDownTimer)
		int CountDownTimer;

private:
	UFUNCTION()
		void StartCountDown();

	UFUNCTION()
		void OnRep_RoundEnded();

	UFUNCTION()
		void OnRep_CountDownTimer();

	FTimerHandle CountDownTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* TimerEndSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* TimerTickSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* GameEndSound;
};
