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

protected:
	UPROPERTY()
		TArray<APlayerStart*> PlayerStarts;
};
