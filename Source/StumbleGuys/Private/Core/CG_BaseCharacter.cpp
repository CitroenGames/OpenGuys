// CitroenGames 2023

#include "Core/CG_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACG_BaseCharacter::ACG_BaseCharacter()
{
	SetReplicates(true);
	PlayerStateRef = Cast<ACG_PlayerState>(GetPlayerState());
	// set mesh replicated to true
	GetMesh()->SetIsReplicated(true);
	DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	DynamicMaterial1 = GetMesh()->CreateAndSetMaterialInstanceDynamic(1);
}

// Called when the game starts or when spawned
void ACG_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnPoof();
	// if locally controlled
	if (IsLocallyControlled())
	{

	}
}

// Called to bind functionality to input
void ACG_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACG_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ACG_BaseCharacter::SpawnPoof()
{
	// Spawn Niagra particle effect at actor location

	if (SpawnNiagaraParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnNiagaraParticle, GetActorLocation());
	}

	// Play sound effect at actor location
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSoundEffect, GetActorLocation());

}

void ACG_BaseCharacter::Server_SetAppearance_Implementation(bool AppearanceMale, int NewAppearanceColorID)
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