// Fill out your copyright notice in the Description page of Project Settings.


#include "MySpectralPlane.h"

// Sets default values
AMySpectralPlane::AMySpectralPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// создаем и крепим меш
	DoorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DoorRoot;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(DoorRoot);

	// Настраиваем коллизию, чтобы LineTrace (ECC_Visibility) его видел
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

}

// Called when the game starts or when spawned
void AMySpectralPlane::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMySpectralPlane::OnInteract_Implementation(AMyBaseCharacter* Interactor)
{



	OnInteracted();
}

// Called every frame
void AMySpectralPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

