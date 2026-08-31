// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterFactionType.generated.h"

UENUM(NotBlueprintType)
enum class ECharacterFaction : uint8
{
	PlayerFriend,
	Neutral,
	Enemy,
	None
};
