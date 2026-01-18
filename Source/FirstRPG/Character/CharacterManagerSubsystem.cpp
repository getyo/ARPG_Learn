// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterManagerSubsystem.h"


void UCharacterManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}
void UCharacterManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FString UCharacterManagerSubsystem::GenerateID(AGeneralCharacter *Character)
{
	FScopeLock Lock(&IDMutex);
	FGuid Guid = FGuid::NewGuid();
	FString ID = Guid.ToString();
	ID2CharcCharacterMap.Add(ID,Character);
	Name2IDMap.Add(Character->CharacterName,ID);
	return ID;
}
