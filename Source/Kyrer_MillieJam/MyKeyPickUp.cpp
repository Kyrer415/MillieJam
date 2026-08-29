// Fill out your copyright notice in the Description page of Project Settings.

#include "MyKeyPickUp.h"
#include "MyBaseCharacter.h"

// Sets default values
AMyKeyPickUp::AMyKeyPickUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyKeyPickUp::BeginPlay()
{
	Super::BeginPlay();

}

void AMyKeyPickUp::OnInteract_Implementation(AMyBaseCharacter* Interactor)
{
	if (!IsValid(Interactor)) return;
	Interactor->AddKeyItem(KeyItemData);
	Destroy();
}

// Called every frame
void AMyKeyPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

