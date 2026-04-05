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

UENUM(BlueprintType)
enum class E_WeaponKind : uint8
{
	None,
	MeleeWeapon,
	RangeWeapon
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	AWeapon * UsingWeapon = nullptr;
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
	//注意当你装备近战武器时，默认会把它设置为UsingWeapon
	bool SetEquippedMeleeWeapon(UEquipmentInstance * Instance);
	UFUNCTION(BlueprintCallable, Category="Equipment")
	inline void RemoveMeleeWeapon();
	
	//这个函数是其他四个Set装备的包装，可以根据传入的类型来调用相关Set函数
	UFUNCTION(BlueprintCallable, Category="Equipment")
	bool SetEquippedEquipment(UEquipmentInstance * Instance);
	
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
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	bool DrawWeapon(E_WeaponKind WeaponKind);
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void SheatheWeapon();
	UFUNCTION(BlueprintCallable, BlueprintPure,Category="Equipment")
	FORCEINLINE E_WeaponKind GetUsingWeaponKind();
	
	bool ChangeUsingWeapon(UEquipmentInstance * Instance);
	//销毁当前武装武器并且重新将其生成在装备位置
	void DestroyUsingWeapon();

	
	
private:
	template<typename T>
	T* SpawnAtEquippedSocket(UEquipmentInstance* Instance, USceneComponent* ParentMesh)
	{
		if (!Instance || !ParentMesh) return nullptr;
		FTransform SlotTransform = ParentMesh->GetSocketTransform(FName(Instance->GetEquippedSocketName()), RTS_World);
		
		T* EquipmentObj = ParentMesh->GetWorld()->SpawnActorDeferred<T>(Instance->GetBPClassToSpawn(), SlotTransform);
		if (EquipmentObj)
		{
			EquipmentObj->SetItemInstance(Instance);
			EquipmentObj->FinishSpawning(SlotTransform);
			
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
			EquipmentObj->AttachToComponent(ParentMesh, AttachRules, FName(Instance->GetEquippedSocketName()));
			EquipmentObj->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("EquippedWeapon"));
			EquipmentObj->SetOwner(this->GetOwner());
        
			return EquipmentObj;
		}
		return nullptr;
	}
	template<typename T>
	T* SpawnAtArmedSocket(UEquipmentInstance* Instance, USceneComponent* ParentMesh)
	{
		if (!Instance || !ParentMesh) return nullptr;
		FTransform SlotTransform = ParentMesh->GetSocketTransform(FName(Instance->GetArmedSocketName()), RTS_World);
		
		T* EquipmentObj = ParentMesh->GetWorld()->SpawnActorDeferred<T>(Instance->GetBPClassToSpawn(), SlotTransform);
		if (EquipmentObj)
		{
			EquipmentObj->SetItemInstance(Instance);
			EquipmentObj->FinishSpawning(SlotTransform);
			
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
			EquipmentObj->AttachToComponent(ParentMesh, AttachRules, FName(Instance->GetArmedSocketName()));
			EquipmentObj->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("EquippedWeapon"));
			EquipmentObj->SetOwner(this->GetOwner());
        
			return EquipmentObj;
		}
		return nullptr;
	}
};
