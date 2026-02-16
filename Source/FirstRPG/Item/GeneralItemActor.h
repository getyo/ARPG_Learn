// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneralItemInstance.h"
#include "GeneralItemActor.generated.h"

class FIRSTRPG_API UItemManagerSubSystem;


UCLASS(Abstract)
class FIRSTRPG_API AGeneralItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneralItemActor();
	UFUNCTION(BlueprintCallable, Category = "Item")
	FString GetItemInfo() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	UGeneralItemInstance * GetItemInstance()const{return ItemInstance;}
	UFUNCTION(BlueprintCallable, Category = "Item")
	inline void SetItemInstance(UGeneralItemInstance * Instance){ItemInstance = Instance;}
	virtual UGeneralItemInstance * GeneralItemInstance();
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	FDataTableRowHandle ItemInfoHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UGeneralItemInstance * ItemInstance;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Tick(float DeltaTime);
	
};
