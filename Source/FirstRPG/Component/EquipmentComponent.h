// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FirstRPG/Item//Equipment/MeleeWeapon.h"
#include "FirstRPG/Item//Equipment/RangeWeapon.h"
#include "FirstRPG/Item//Equipment/Armor.h"
#include "FirstRPG/Item//Equipment/Shield.h"
#include "EquipmentComponent.generated.h"

USTRUCT(BlueprintType)
struct FS_EquipmentStatus
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Equipment")
	AMeleeWeapon * MeleeWeapon = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Equipment")
	ARangeWeapon * RangeWeapon = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Equipment")
	AShield * Shield = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Equipment")
	AArmor * Armor = nullptr;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ,Blueprintable)
class FIRSTRPG_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	FS_EquipmentStatus EquipmentStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	TArray<UEquipmentInstance*> EquippableItems;
	//仅在C++内部可见，用来加快查找
	TMap<FString,UEquipmentInstance*> EquipmentName2Obj;
	UMeshComponent * OwnerMesh = nullptr;
	USceneCaptureComponent2D * CaptureComponent = nullptr;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category="Equipment")
	inline void AddEquippableItem(AEquippableItemActor* EquippableItem);
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	bool SetEquippedMeleeWeapon(UEquipmentInstance * Instance);
	UFUNCTION(BlueprintCallable, Category="Equipment")
	inline void RemoveMeleeWeapon();
	
	//这个函数是其他四个Set装备的包装，可以根据传入的类型来调用相关Set函数
	UFUNCTION(BlueprintCallable, Category="Equipment")
	bool SetEquipedEquipment(UEquipmentInstance * Instance);
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	bool SetEquippedRangeWeapon(UEquipmentInstance* Instance);
	UFUNCTION(BlueprintCallable, Category="Equipment")
	inline void RemoveRangeWeapon();
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	bool SetEquippedShield(UEquipmentInstance* Instance);
	UFUNCTION(BlueprintCallable, Category="Equipment")
	inline void RemoveShield();
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	inline void RemoveArmor() { EquipmentStatus.Armor = nullptr; }
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	bool SetEquippedArmor(UEquipmentInstance * Instance);
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category="Equipment")
	TArray<UEquipmentInstance*> GetEquippableItems();
private:
	template<typename T>
	T* SpawnAndAttach(UEquipmentInstance* Instance, USceneComponent* ParentMesh)
	{
		if (!Instance || !ParentMesh) return nullptr;
		FTransform SlotTransform = ParentMesh->GetSocketTransform(FName(Instance->GetSocketName()), RTS_World);
		
		T* EquipmentObj = ParentMesh->GetWorld()->SpawnActorDeferred<T>(T::StaticClass(), SlotTransform);
		if (EquipmentObj)
		{
			EquipmentObj->SetItemInstance(Instance);
			EquipmentObj->FinishSpawning(SlotTransform);
			
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
			EquipmentObj->AttachToComponent(ParentMesh, AttachRules, FName(Instance->GetSocketName()));
			EquipmentObj->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("EquippedWeapon"));
        
			return EquipmentObj;
		}
		return nullptr;
	}
};
