// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralCharacter.h"
#include "FirstRPG/Component/CharacterStatusComponent.h"
#include "CharacterManagerSubsystem.h"
#include "FirstRPG/Debug/Debug.h"
#include "FirstRPG/Item/Equipment/MeleeWeapon.h"
#include "FirstRPG/Item/Equipment/RangeWeapon.h"
#include "FirstRPG/Item/Equipment/Shield.h"
#include "FirstRPG/SaveSystem/SaveLoadSystem.h"

AGeneralCharacter::AGeneralCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGeneralCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// 只在没有 ID 时生成：编辑器里摆好的 actor 一旦存过关卡就会一直是同一个 ID
	if (CharacterID.IsEmpty())
	{
		FGuid Guid = FGuid::NewGuid();
		CharacterID = Guid.ToString();
	}
}

// Called when the game starts or when spawned
void AGeneralCharacter::BeginPlay()
{
	Super::BeginPlay();
	RegisterCharacter();
}

void AGeneralCharacter::PreDuplicate(FObjectDuplicationParameters& DupParams)
{
	Super::PreDuplicate(DupParams);
	//重新生成ID并注册
	FGuid Guid = FGuid::NewGuid();
	CharacterID = Guid.ToString();
}

void AGeneralCharacter::RegisterCharacter() 
{
	// 运行时兜底：老关卡里没存过 ID 的 actor 保证能拿到唯一 ID
	if (CharacterID.IsEmpty())
	{
		CharacterID = FGuid::NewGuid().ToString();
	}
	UGameInstance* GI = GetGameInstance();
	if (GI)
	{
		// 角色管理子系统注册
		if (UCharacterManagerSubsystem* CharacterSys = GI->GetSubsystem<UCharacterManagerSubsystem>())
		{
			CharacterSys->Register(this);
		}
		//存档子系统注册
		if (USaveLoadSystem* SaveLoadSystem = GI->GetSubsystem<USaveLoadSystem>())
		{
			SaveLoadSystem->Register(this);
		}
	}
}

void AGeneralCharacter::UnRegisterCharacter()
{
	UGameInstance* GI = GetGameInstance();
	if (GI)
	{
		// 角色管理子系统注册
		if (UCharacterManagerSubsystem* CharacterSys = GI->GetSubsystem<UCharacterManagerSubsystem>())
		{
			CharacterSys->UnRegister(this);
		}
		//存档子系统注册
		if (USaveLoadSystem* SaveLoadSystem = GI->GetSubsystem<USaveLoadSystem>())
		{
			SaveLoadSystem->UnRegister(this);
		}
	}
}

// Called every frame
void AGeneralCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AGeneralCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

AGeneralCharacter* AGeneralCharacter::SpawnCharacter(UObject* WorldContextObject,
                                                     TSubclassOf<AGeneralCharacter> CharacterClass,
                                                     FTransform SpawnTransform)
{
	if (!WorldContextObject || !CharacterClass) return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;
	
	auto Character = World->SpawnActorDeferred<AGeneralCharacter>(CharacterClass, SpawnTransform);
	if (Character)
	{
		Character->IsStatic = false;
		Character->FinishSpawning(SpawnTransform);
	}

	return Character;
}

void AGeneralCharacter::SaveCharacterData_Implementation(FCharacterSaveData& Data)
{
	// 装备组件是可选的（马/动物/怪物等没有装备）：没有也要把世界信息写全，
	// 否则整条数据是默认值（class 空、IsStatic=false），读档时会被误当成“动态角色”去重建
	if (auto* EquipmentCmp = FindComponentByClass<UEquipmentComponent>())
	{
		auto EStatus = EquipmentCmp->GetCurEquipmentStatus();
		if (EStatus.MeleeWeapon) Data.EquipStatus.MeleeWeapon = EStatus.MeleeWeapon->GetItemInfoHandle();
		if (EStatus.RangeWeapon) Data.EquipStatus.RangeWeapon = EStatus.RangeWeapon->GetItemInfoHandle();
		if (EStatus.Shield) Data.EquipStatus.Shield = EStatus.Shield->GetItemInfoHandle();
		if (EStatus.Armor) Data.EquipStatus.Armor = EStatus.Armor->GetItemInfoHandle();
	}
	// 状态数据（生命/耐力/等级）由 C++ 状态组件提供
	if (auto* StatusCmp = FindComponentByClass<UCharacterStatusComponent>())
	{
		Data.Health = StatusCmp->CurHealth;
		Data.MaxHealth = StatusCmp->MaxHealth;
		Data.MaxStamina = StatusCmp->MaxStamina;
		Data.Level = StatusCmp->Level;
		Data.CanBeKilled = StatusCmp->CanBeKilled;
	}
	Data.Faction = CharacterFaction;
	Data.IsStatic = IsStatic;
	Data.Name = CharacterName;
	// 世界信息对齐 AGeneralItemActor::SaveItemData（不然重建时 class 为空、位置落在 0,0,0）
	Data.CharacterClass = GetClass();
	const FTransform Transform = GetActorTransform();
	Data.Location = Transform.GetLocation();
	Data.Rotation = Transform.GetRotation().Rotator();
	Data.Scale = Transform.GetScale3D();
}

void AGeneralCharacter::LoadCharacterData_Implementation(const FCharacterSaveData& Data)
{
	// 状态恢复（没有装备组件的角色也要执行）
	if (auto* StatusCmp = FindComponentByClass<UCharacterStatusComponent>())
	{
		StatusCmp->MaxHealth = Data.MaxHealth;
		StatusCmp->MaxStamina = Data.MaxStamina;
		StatusCmp->Level = Data.Level;
		StatusCmp->CanBeKilled = Data.CanBeKilled;
		StatusCmp->CurHealth = FMath::Clamp(Data.Health, 0.f, Data.MaxHealth);
		StatusCmp->CurStamina = StatusCmp->MaxStamina;
		StatusCmp->IsDead = StatusCmp->CurHealth <= 0.f;
		StatusCmp->BroadcastStatusValues();
	}
	CharacterFaction = Data.Faction;

	// 装备：组件是可选的（马/动物等没有就跳过）
	auto EquipmentCmp = FindComponentByClass<UEquipmentComponent>();
	if (!EquipmentCmp)
	{
		return;
	}
	if (!Data.EquipStatus.MeleeWeapon.IsNull())
	{
		auto MeleeInst = UEquipmentInstance::EquipmentInstanceFactory(Data.EquipStatus.MeleeWeapon);
		EquipmentCmp->AddEquippableItem(MeleeInst);
		EquipmentCmp->SetEquippedMeleeWeapon(MeleeInst);
	}
	if (!Data.EquipStatus.RangeWeapon.IsNull())
	{
		auto RangeInst = UEquipmentInstance::EquipmentInstanceFactory(Data.EquipStatus.RangeWeapon);
		EquipmentCmp->AddEquippableItem(RangeInst);
		EquipmentCmp->SetEquippedRangeWeapon(RangeInst);
	}
	if (!Data.EquipStatus.Armor.IsNull())
	{
		auto ArmorInts = UEquipmentInstance::EquipmentInstanceFactory(Data.EquipStatus.Armor);
		EquipmentCmp->AddEquippableItem(ArmorInts);
		EquipmentCmp->SetEquippedArmor(ArmorInts);
	}
	if (!Data.EquipStatus.Shield.IsNull())
	{
		auto ShieldInst = UEquipmentInstance::EquipmentInstanceFactory(Data.EquipStatus.Shield);
		EquipmentCmp->AddEquippableItem(ShieldInst);
		EquipmentCmp->SetEquippedShield(ShieldInst);
	}
}

void AGeneralCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnRegisterCharacter();
}


