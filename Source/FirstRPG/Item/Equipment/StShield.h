#pragma once

#include "CoreMinimal.h"
#include "Shield.h"
#include "EquipmentInstance.h"
#include "StShield.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTRPG_API AStShield : public AShield
{
	GENERATED_BODY()
	AStShield();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="StaticMesh")
	UStaticMeshComponent* MeshComp;
public:
	UStaticMeshComponent* GetStaticMeshComponent() const{return MeshComp;}
};
