// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBowl.h"

// Sets default values
AMyBowl::AMyBowl()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// создаем и крепим меш
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Настраиваем коллизию, чтобы LineTrace (ECC_Visibility) его видел
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
}

void AMyBowl::OnInteract_Implementation(class AMyBaseCharacter* Interactor)
{
	// прозваниваем ивент
	OnInteracted();
}

// Called when the game starts or when spawned
void AMyBowl::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBowl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

