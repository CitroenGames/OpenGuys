// CitroenGames 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CG_PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCG_PlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STUMBLEGUYS_API ICG_PlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// interface for when player collides with an actor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CG_PlayerInterface")
		void OnPlayerCollision(AActor* CollidedActor);
};
