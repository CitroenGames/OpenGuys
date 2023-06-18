// CitroenGames 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CG_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STUMBLEGUYS_API ACG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public: 

	ACG_PlayerController();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Spawn Character")
		void ServerSpawnCharacter();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Spawn Character")
		void ServerSpawnSpectator(bool SpawnAtPlayerLocation);
	
protected:
	// player state class
	UPROPERTY(EditDefaultsOnly, Category = "Player State")
		class ACG_PlayerState* PlayerStateRefrence;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Player State")
		bool IsPlaying = false;
		
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// override end play
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
		void InitPlayerController();
};
