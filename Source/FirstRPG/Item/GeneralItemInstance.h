// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GeneralItemInstance.generated.h"

USTRUCT(blueprintType)
struct FS_GeneralItemInfo :public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	FString ItemName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	FString DisplayName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	UTexture2D* Icon = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	UStaticMesh* StMesh = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	USkeletalMesh* SkMesh = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	int32 PickUpCnt = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	AActor* OwnerActor = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	bool CanBeStacked = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	TSubclassOf<class AGeneralItemActor> BPClassToSpawn = nullptr;
};


UCLASS()
class FIRSTRPG_API UGeneralItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UGeneralItemInstance();
	
protected:
	//Name准确来说应该是物品类型，因为一把剑的类，可以有多个对象，但是名字都是剑
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	FString ItemName;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	FString DisplayName;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	AActor* OwnerActor;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	UTexture2D* Icon;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	UStaticMesh* StMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	USkeletalMesh* SkMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	FString ItemID;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	int32 TotalCnt = 0;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	bool _CanBeStacked = false;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	TSubclassOf<class AGeneralItemActor> _BPClassToSpawn = nullptr;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item")
	FDataTableRowHandle DataTableRow;
public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	AActor * Holder = nullptr;
	
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Item")
	inline FString GetItemID() const{return ItemID;}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Item")
	inline FString GetItemName() const{return DisplayName;}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Item")
	inline FString GetDisplayName() const{return DisplayName;}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Item")
	inline UStaticMesh*GetStMesh() const{return StMesh;}
	inline USkeletalMesh*GetSkMesh() const{return SkMesh;}
	inline int32 GetTotalCnt() const{return TotalCnt;}
	inline bool CanBeStacked() const{return _CanBeStacked;}
	inline FString GetItemInfo() const
	{
		FString ItemInfo = FString::Printf(TEXT("ItemName:%s , DisplayName:%s"),
		*ItemID,*DisplayName);
		return ItemInfo;
	}
	inline TSubclassOf<class AGeneralItemActor> GetBPClassToSpawn() const
	{
		return _BPClassToSpawn;
	}
	inline FDataTableRowHandle GetDataTableRow() const{return DataTableRow;}
	virtual void Initialize(FS_GeneralItemInfo *ItemInfo);
	UFUNCTION(BlueprintCallable,Category = "Item")
	void AddCnt(int Cnt);
	UFUNCTION(BlueprintCallable,Category = "Item")
	static UGeneralItemInstance * GeneralItemInstanceFactory(const FDataTableRowHandle &DataSource);
};
