#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneralItemInstance.h"
#include "FirstRPG/SaveSystem/SaveGameInterface.h"
#include "GeneralItemActor.generated.h"

class FIRSTRPG_API UItemManagerSubSystem;


UCLASS(Abstract)
class FIRSTRPG_API AGeneralItemActor : public AActor,public IItemSaveInterface
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
	UFUNCTION(BlueprintCallable, Category = "Item")
	inline FString GetItemID() const{return ItemID;}
	UFUNCTION(BlueprintCallable, Category = "Item")
	inline FDataTableRowHandle GetItemInfoHandle() const{return ItemInfoHandle;}
	virtual void SaveItemData_Implementation(FItemSaveData& Data) override;
	virtual void LoadItemData_Implementation(const FItemSaveData& Data) override;
	UFUNCTION(BlueprintCallable, Category = "Item")
	static AGeneralItemActor* SpawnItem(UObject* WorldContextObject, TSubclassOf<AGeneralItemActor> ItemClass,
	                                    UGeneralItemInstance* ItemInst, FTransform SpawnTransform);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
	FDataTableRowHandle ItemInfoHandle;
	
	UGeneralItemInstance * ItemInstance;
	bool IsStatic = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	bool Dead = false;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PreDuplicate(FObjectDuplicationParameters& DupParams) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Tick(float DeltaTime);
	void RegisterItem();
	void UnRegisterItem();
private:
	FString ItemID;
};
