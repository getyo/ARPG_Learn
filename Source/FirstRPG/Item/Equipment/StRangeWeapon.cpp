#include "StRangeWeapon.h"
#include "FirstRPG/Debug/Debug.h"

AStRangeWeapon::AStRangeWeapon() : ARangeWeapon()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));;
	SetRootComponent(MeshComp);
}


void AStRangeWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (ItemInstance->GetStMesh())
	{
		MeshComp->SetStaticMesh(ItemInstance->GetStMesh());
		MeshComp->SetCollisionProfileName("PickableItem");
	}
	else
	{
		CPP_LOG(Error,"Mesh is not StaticMesh");
		return;
	}
}
