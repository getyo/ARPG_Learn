#include "SkArmor.h"
#include "FirstRPG/Debug/Debug.h"

ASkArmor::ASkArmor() : AArmor()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));;
	SetRootComponent(MeshComp);
}


void ASkArmor::BeginPlay()
{
	Super::BeginPlay();
	if (ItemInstance->GetSkMesh())
	{
		MeshComp->SetSkeletalMesh(ItemInstance->GetSkMesh());
		MeshComp->SetCollisionProfileName("PickableItem");
	}
	else
	{
		CPP_LOG(Error,"Mesh is not SkMesh");
		return;
	}
}
