// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FirstRPG/Item/Equipment/EquipmentDataType.h"
#include "FirstRPG/Item/Equipment/EquipmentInstance.h"
#include "FirstRPG/Item/Equipment/Weapon.h"
#include "FirstRPG/Item/Equipment/Armor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "EquipmentComponent.generated.h"


//武装状态的单一真源：抢、盾、双手武器都由它推导，避免多个布尔各自为政
UENUM(BlueprintType)
enum class E_ArmedState : uint8
{
	None,
	MeleeArmed,
	RangeArmed
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArmedStateChanged, E_ArmedState, NewState);

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
	//这个函数是其他四个Set装备的包装，可以根据传入的类型来调用相关Set函数
	bool SetEquippedEquipment(UEquipmentInstance * Instance);
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//在EquippedSlot生成近战武器网格体
	//注意当你装备近战武器时，默认会把它设置为UsingWeapon
	bool SetEquippedMeleeWeapon(UEquipmentInstance * Instance);
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//销毁当前近战武器网格体实例（无论武装/装备状态），不对Inst进行销毁
	inline void RemoveMeleeWeapon();
	
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//在EquippedSlot生成远程武器网格体
	bool SetEquippedRangeWeapon(UEquipmentInstance* Instance);
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//销毁当前远程武器网格体实例（无论武装/装备状态），不对Inst进行销毁
	inline void RemoveRangeWeapon();
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//在EquippedSlot生成盾牌网格体
	bool SetEquippedShield(UEquipmentInstance* Instance);
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//销毁当前盾牌网格体实例（无论武装/装备状态），不对Inst进行销毁
	inline void RemoveShield();
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//销毁当前盔甲网格体实例，不对Inst进行销毁
	inline void RemoveArmor();
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//生成盔甲网格体
	bool SetEquippedArmor(UEquipmentInstance * Instance);
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category="Equipment")
	TArray<UEquipmentInstance*> GetEquippableItems();
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//销毁当前装备武器并重新生成在武装位置上，与DestroyUsingWeapon作用相反
	bool DrawWeapon(E_WeaponKind WeaponKind);
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//销毁EquippedSlot盾牌，在ArmedSlot重新生成网格体
	bool DrawShield();
	UFUNCTION(BlueprintCallable, Category="Equipment")
	//DestroyUsingWeapon的对外封装
	void SheatheWeapon();
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void SheatheShield();
	UFUNCTION(BlueprintCallable, BlueprintPure,Category="Equipment")
	FORCEINLINE E_WeaponKind GetUsingWeaponKind();

	//===== 武装状态单一真源 =====
	//当前武装状态（None/近战/远程），所有 Draw/Sheathe/SetEquipped 都会同步它
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Equipment")
	E_ArmedState GetArmedState() const { return ArmedState; }
	//盾牌是否处于手持（武装）状态
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Equipment")
	bool IsShieldArmed() const { return bShieldArmed; }
	//是否正持有双手武器（此时不允许盾牌上手）
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Equipment")
	bool IsTwoHandedArmed() const { return ArmedState == E_ArmedState::RangeArmed; }
	//强制手占用规则：双手武器在手上时，盾牌必须收回装备位
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void EnforceHandRules();
	//武装状态变化时广播，供蓝图/动画层订阅
	UPROPERTY(BlueprintAssignable, Category="Equipment")
	FOnArmedStateChanged OnArmedStateChanged;
	
	//DrawWeapon的下游，负责根据传入的Inst进行实际网格体生成和销毁
	bool ChangeUsingWeapon(UEquipmentInstance * Instance);
	//销毁当前武装武器并且重新将其生成在装备位置，与DrawWeapon作用相反
	void DestroyUsingWeapon();
	FS_EquipmentRef GetCurEquipmentStatus(){ return CurEquipmentRef;}
	void ClearBag();
	
private:
	//单一真源：当前武装状态 + 盾是否上手
	E_ArmedState ArmedState = E_ArmedState::None;
	bool bShieldArmed = false;
	void SetArmedState(E_ArmedState NewState);

	template<typename T>
	T* SpawnAtEquippedSocket(UEquipmentInstance* Instance, USceneComponent* ParentMesh)
	{
		if (!Instance || !ParentMesh) return nullptr;
		FTransform SlotTransform = ParentMesh->GetSocketTransform(FName(Instance->GetEquippedSocketName()), RTS_World);
		
		T* EquipmentObj = Cast<T>(AGeneralItemActor::SpawnItem(ParentMesh->GetWorld(), Instance->GetBPClassToSpawn(), Instance, SlotTransform));
		if (EquipmentObj)
		{
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
		
		T* EquipmentObj = Cast<T>(AGeneralItemActor::SpawnItem(ParentMesh->GetWorld(), Instance->GetBPClassToSpawn(), Instance, SlotTransform));
		if (EquipmentObj)
		{
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
