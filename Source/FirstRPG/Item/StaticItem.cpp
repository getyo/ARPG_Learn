// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticItem.h"

#include "DataWrappers/ChaosVDParticleDataWrapper.h"
#include "FirstRPG/Debug/Debug.h"

void AStaticItem::BeginPlay()
{
	Super::BeginPlay();
	if (ItemInstance->GetStMesh())
	{
		MeshComp->SetStaticMesh(ItemInstance->GetStMesh());
	}
	else
	{
		CPP_LOG(Error,"Mesh is not StaticMesh");
	}
}
