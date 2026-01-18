// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"
#include "FirstRPG/Character//GeneralCharacter.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (GI)
	{
		DialogueSubsystem= GI->GetSubsystem<UDialogueSubsystem>();
		if (!DialogueSubsystem)
		{
			GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
				FString::Printf(TEXT("Class : %s,Cannot get Dialogue SubSystem"),
					*GetName()));
		}
	}
	
}

