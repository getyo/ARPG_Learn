// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralCharacter.h"
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
	FGuid Guid = FGuid::NewGuid();
	CharacterID = Guid.ToString();
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
	auto EquipmentCmp = FindComponentByClass<UEquipmentComponent>();
	if (!EquipmentCmp)
	{
		CPP_LOG(Error, TEXT("Equipment component not found"));
	}
	auto EStatus = EquipmentCmp->GetCurEquipmentStatus();
	if (EStatus.MeleeWeapon) Data.EquipStatus.MeleeWeapon = EStatus.MeleeWeapon->GetItemInfoHandle();
	if (EStatus.RangeWeapon) Data.EquipStatus.RangeWeapon = EStatus.RangeWeapon->GetItemInfoHandle();
	if (EStatus.Shield) Data.EquipStatus.Shield = EStatus.Shield->GetItemInfoHandle();
	if (EStatus.Armor) Data.EquipStatus.Armor = EStatus.Armor->GetItemInfoHandle();
	Data.IsStatic = IsStatic;
	Data.Name = CharacterName;
}

void AGeneralCharacter::LoadCharacterData_Implementation(const FCharacterSaveData& Data)
{
	auto EquipmentCmp = FindComponentByClass<UEquipmentComponent>();
	if (!EquipmentCmp)
	{
		CPP_LOG(Error, TEXT("Equipment component not found"));
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
		EquipmentCmp->SetEquippedMeleeWeapon(RangeInst);
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
		EquipmentCmp->SetEquippedMeleeWeapon(ShieldInst);
	}
	IsStatic = Data.IsStatic;
}

void AGeneralCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnRegisterCharacter();
}


