// CitroenGames 2023

#include "Core/CG_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACG_BaseCharacter::ACG_BaseCharacter()
{
	SetReplicates(true);
	PlayerStateRef = Cast<ACG_PlayerState>(GetPlayerState());
	// set mesh replicated to true
	GetMesh()->SetIsReplicated(true);
	DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	DynamicMaterial1 = GetMesh()->CreateAndSetMaterialInstanceDynamic(1);

	// camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	// camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ACG_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawningEffects();
	// if locally controlled
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

void ACG_BaseCharacter::ServerSetAppearance_Implementation(bool AppearanceMale, int NewAppearanceColorID)
{
	int ColorID = NewAppearanceColorID;
	GetMesh()->SetSkeletalMesh(AppearanceMale ? MaleMesh : FemaleMesh);

	if (DynamicMaterial == nullptr)
	{
		DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	}

	// set mesh material 
	DynamicMaterial->SetVectorParameterValue("Character_Color", GetColorFromID(ColorID));

	if (DynamicMaterial1 == nullptr)
	{
		DynamicMaterial1 = GetMesh()->CreateAndSetMaterialInstanceDynamic(1);
	}

	DynamicMaterial1->SetVectorParameterValue("Character_Color", GetColorFromID(ColorID));

	// apply the material to the mesh
	GetMesh()->SetMaterial(0, DynamicMaterial);
	GetMesh()->SetMaterial(1, DynamicMaterial1);
}

FLinearColor ACG_BaseCharacter::GetColorFromID(int ColorID)
{
	// return the color from the color id
	switch (ColorID)
	{
		case 0:
			return FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
		case 1:
			return FLinearColor(0.0f, 0.0f, 1.0f, 1.0f);
		case 2:
			return FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
		case 3:
			return FLinearColor(0.0f, 1.0f, 1.0f, 1.0f);
		case 4:
			return FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
		case 5:
			return FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);
		case 6:
			return FLinearColor(1.0f, 1.0f, 0.0f, 1.0f);
		case 7:
			return FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
		default:
			return FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

void ACG_BaseCharacter::SetZoomDistanceCamera(float NewZoomDistance)
{
	if (CameraBoom)
	{
		// Get current spring arm length and add the new zoom distance
		float NewCameraArmLength = CameraBoom->TargetArmLength + NewZoomDistance;

		// Clamp the new camera arm length between 100 and 1500
		NewCameraArmLength = FMath::Clamp(NewCameraArmLength, 100.0f, 1500.0f);

		// Set the new spring arm length
		CameraBoom->TargetArmLength = NewCameraArmLength;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CameraBoom is not initialized."));
	}
}
