// Fill out your copyright notice in the Description page of Project Settings.


#include "StMeleeWeapon.h"
#include "FirstRPG/Debug/Debug.h"

AStMeleeWeapon::AStMeleeWeapon() : AMeleeWeapon()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));;
	SetRootComponent(MeshComp);
}


void AStMeleeWeapon::BeginPlay()
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
