// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NativeGameplayTags.h"
#include "GeneralActor.generated.h"

UCLASS()
class FIRSTRPG_API AGeneralActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneralActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	FGameplayTag ItemTag;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	FString ItemName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	FString DisplayName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	AActor* OwnerActor;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FString GetItemID() const{return ItemID;}
	inline FString GetItemName() const{return DisplayName;}
	inline FString GetDisplayName() const{return DisplayName;}
	inline FGameplayTag GetItemTag() const{return ItemTag;}
private:
	FString ItemID;

};
