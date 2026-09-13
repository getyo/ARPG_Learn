#include "StShield.h"
#include "FirstRPG/Debug/Debug.h"

AStShield::AStShield() : AShield()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));;
	SetRootComponent(MeshComp);
}


void AStShield::BeginPlay()
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