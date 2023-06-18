// CitroenGames 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CG_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STUMBLEGUYS_API ACG_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACG_PlayerState();
	
protected:

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Replicated)
		bool AppearanceIsInit;

	UPROPERTY(BlueprintReadWrite, Replicated)
		FName CurrentCheckPoint;

	UPROPERTY(BlueprintReadOnly, Replicated)
		int AppearanceColorID;

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		bool bAppearanceMale;
};
