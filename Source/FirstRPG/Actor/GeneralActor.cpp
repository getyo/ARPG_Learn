// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralActor.h"

// Sets default values
AGeneralActor::AGeneralActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGeneralActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGeneralActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

