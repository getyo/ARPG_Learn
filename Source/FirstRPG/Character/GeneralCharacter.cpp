// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralCharacter.h"
#include "CharacterManagerSubsystem.h"

AGeneralCharacter::AGeneralCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGeneralCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UGameInstance* GI = GetGameInstance();
	if (GI)
	{
		UCharacterManagerSubsystem* CharacterSys = GI->GetSubsystem<UCharacterManagerSubsystem>();
    
		// 使用子系统
		if (CharacterSys)
		{
			CharacterID = CharacterSys->GenerateID(this);
		}
	}
}

inline  bool AGeneralCharacter::IsQuestRelative_Implementation()
{
	return QuestRelative;
}

TArray<FString> AGeneralCharacter::GetRelativeQuest_Implementation()
{
	return TArray<FString>();
}

int AGeneralCharacter::GetRelativeQuestStage_Implementation(const FString& QuestID)
{
	return -1;
}

// Called every frame
void AGeneralCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AGeneralCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

