// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPickableItem.h"

// Sets default values
AMyPickableItem::AMyPickableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// создаем и крепим меш
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Настраиваем коллизию, чтобы LineTrace (ECC_Visibility) его видел
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

}

void AMyPickableItem::OnInteract_Implementation(class AMyBaseCharacter* Interactor)
{
	if (!IsValid(Interactor)) return;

	if (bIsFood)
	{
		// если еда - даём 1 единицу корма
		Interactor->AddFood(FoodAmount);
	}
	else
	{
		// Eсли предмет - передаём всю структуру данных
		Interactor->AddKeyItem(ItemData);
	}
	
	// Самоуничтожение предмета
	Destroy();
}

// Called when the game starts or when spawned
void AMyPickableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

