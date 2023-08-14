// CitroenGames 2023

#include "Core/CG_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Core/CG_PlayerController.h"

// Sets default values
ACG_BaseCharacter::ACG_BaseCharacter()
{
	SetReplicates(true);
	PlayerStateRef = Cast<ACG_PlayerState>(GetPlayerState());
	// set mesh replicated to true
	GetMesh()->SetIsReplicated(true);
}

void ACG_BaseCharacter::RespawnPlayer()
{
	// get player controller and then call respawn function ACG_PlayerController::ServerRespawnPlayer
	ACG_PlayerController* PlayerController = Cast<ACG_PlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->ServerSpawnCharacter();
	}
}

// Called when the game starts or when spawned
void ACG_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawningEffects();
	// if locally controlled
	// TODO This needs a ton of work
	if (IsLocallyControlled())
	{
		
	}
}

// Is called when the player is colliding with another actor
void ACG_BaseCharacter::OnPlayerCollision_Implementation(AActor* CollidedActor)
{
	// if the player is not ragdolling
	if (!bIsRagdolling)
	{
		// ragdoll the player
		bIsRagdolling = true;
		Multicast_Ragdoll();
		UE_LOG(LogTemp, Warning, TEXT("Ragdolling"));
	}
}

// ragdoll function
void ACG_BaseCharacter::Multicast_Ragdoll_Implementation()
{
	// ragdoll the player
	bIsRagdolling = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	UE_LOG(LogTemp, Warning, TEXT("Ragdolling"));
	// 2 second timer to respawn the player
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACG_BaseCharacter::RespawnPlayer, 2.0f, false);
}

// Called to bind functionality to input
void ACG_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// replication function
void ACG_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Spawn Effects
void ACG_BaseCharacter::SpawningEffects()
{
	// Spawn Niagra particle effect at actor location
	if (SpawnNiagaraParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnNiagaraParticle, GetActorLocation());
	}

	// Play sound effect at actor location
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSoundEffect, GetActorLocation());
}

void ACG_BaseCharacter::MulticastSetAppearance_Implementation(bool AppearanceMale, int NewAppearanceColorID)
{
}
