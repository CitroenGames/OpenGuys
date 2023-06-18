// CitroenGames 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "CG_PlayerState.h"
#include "CG_PlayerInterface.h"
#include "CG_BaseCharacter.generated.h"

class ACG_PlayerState;

UCLASS()
class STUMBLEGUYS_API ACG_BaseCharacter : public ACharacter, public ICG_PlayerInterface
{
	GENERATED_BODY()

public:

	// player interface 
	virtual void OnPlayerCollision_Implementation(AActor* CollidedActor) override;

	// ragdoll function
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void Multicast_Ragdoll();

	// Sets default values for this character's properties
	ACG_BaseCharacter();

	ACG_PlayerState* PlayerStateRef;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void ServerSetAppearance(bool AppearanceMale, int NewAppearanceColorID);

	// blueprint implementable function for respawning the player
	UFUNCTION(BlueprintImplementableEvent, Category = "CG_BaseCharacter")
		void RespawnPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, Category = "Effects")
		UNiagaraSystem* SpawnNiagaraParticle;

	UPROPERTY(EditAnywhere, Category = "Effects")
		USoundBase* SpawnSoundEffect;

	// male mesh variable for character
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		USkeletalMesh* MaleMesh;

	// female mesh variable for character
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		USkeletalMesh* FemaleMesh;


private:

	UPROPERTY(Replicated)
		bool bIsRagdolling;

	UFUNCTION()
		void SpawnPoof();

	UFUNCTION()
		FLinearColor GetColorFromID(int ColorID);

	UPROPERTY()
		UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
		UMaterialInstanceDynamic* DynamicMaterial1;

};
