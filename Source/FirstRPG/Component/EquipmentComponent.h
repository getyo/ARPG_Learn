// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FirstRPG/Item//Equipment/MeleeWeapon.h"
#include "FirstRPG/Item//Equipment/RangeWeapon.h"
#include "FirstRPG/Item//Equipment/Armor.h"
#include "FirstRPG/Item//Equipment/Shield.h"
#include "Components/SceneCaptureComponent2D.h"
#include "EquipmentComponent.generated.h"

USTRUCT(Blueprintable,BlueprintType)
struct FS_EquipmentRef
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	AMeleeWeapon * MeleeWeapon = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	ARangeWeapon * RangeWeapon = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	AShield * Shield = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	AArmor * Armor = nullptr;
};

USTRUCT(Blueprintable,BlueprintType)
struct FS_DefaultEquipmentStatus
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	FDataTableRowHandle MeleeWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	FDataTableRowHandle RangeWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	FDataTableRowHandle Shield;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	FDataTableRowHandle Armor;
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
	//默认装备，NPC的话没有特殊情况就不变了,和CurEquipmentRef一样
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	FS_DefaultEquipmentStatus DefaultEquipment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	TArray<UEquipmentInstance*> EquippableItems;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	AWeapon * UsingWeapon = nullptr;
	//当前使用装备的ActorRef
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	FS_EquipmentRef CurEquipmentRef;
	
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
	inline void AddEquippableItem(UEquipmentInstance* EquipmentInst);
	
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
	inline void RemoveArmor();
	
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
			if (GetOwner()->ActorHasTag("Player"))
				CaptureComponent->ShowOnlyActors.Add(EquipmentObj);
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
			if (GetOwner()->ActorHasTag("Player"))
				CaptureComponent->ShowOnlyActors.Add(EquipmentObj);
			return EquipmentObj;
		}
		return nullptr;
	}
};
