// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GeneralCharacter.generated.h"

UCLASS(Abstract, Blueprintable,BlueprintType)
class FIRSTRPG_API AGeneralCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGeneralCharacter();
	// Sets default values for this character's properties
	AGeneralCharacter(const FString &CharacterName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FString GetCharacterID() const
	{
		return CharacterID;
	}
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	FString CharacterName = "";
private:
	FString CharacterID = "";
};
