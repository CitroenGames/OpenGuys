// CitroenGames 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "CG_Gamemode.generated.h"

UCLASS()
class STUMBLEGUYS_API ACG_Gamemode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ACG_Gamemode();
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC);

	UFUNCTION(BlueprintCallable, Category = "Spawn Character")
		void SpawnCharacter(APlayerController* PlayerController, FName CheckPoint, int AppearanceID, bool IsMaleAppearance);

	UFUNCTION(BlueprintCallable, Category = "Spawn Character")
		void SpawnSpectator(APlayerController* Player, bool SpawnAtPlayerLocation, FVector SpawnTransform);

	UPROPERTY(EditDefaultsOnly, Category = "Player State")
		UClass* CharacterClass;

	UFUNCTION()
		void RefreshPlayerStarts();

	UFUNCTION()
		void EnableCharacterMovement();

	UPROPERTY(BlueprintReadWrite)
		TArray<APlayerController*> AllPlayerControllers;

	UFUNCTION(BlueprintCallable)
		void TraveltoLevel();

protected:
	UPROPERTY()
		TArray<APlayerStart*> PlayerStarts;

private:

};
