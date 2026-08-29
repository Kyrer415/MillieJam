#include "CombinationItem.h"
#include <array>
#include "MyBaseCharacter.h"

ACombinationItem::ACombinationItem()
{
    PrimaryActorTick.bCanEverTick = false;

    ItemRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ItemRoot"));
    RootComponent = ItemRoot;

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    ItemMesh->SetupAttachment(ItemRoot);
}

void ACombinationItem::BeginPlay()
{
    Super::BeginPlay();

    StartPosition = GetActorLocation();
}

void ACombinationItem::OnInteract_Implementation(AMyBaseCharacter* Interactor)
{
    if (!IsValid(Interactor))
        return;
    if (Interactor->HasItem(OwnItemData.ItemID))
    {
        // тут отправляем в бп объекта чтобы красиво с таймлайном опустить объект на старт позицию
        OnInteractDown(Interactor);
        Interactor->RemKeyItem(OwnItemData);
        bIsRaised = false;
        return;
    }

    if (Interactor->HasItem(FirItemData.ItemID) or Interactor->HasItem(SecItemData.ItemID))
    {
        OnObjectInteracted.Broadcast(this);
        if (Interactor->HasItem(FirItemData.ItemID)) Interactor->RemKeyItem(FirItemData);
        if (Interactor->HasItem(SecItemData.ItemID)) Interactor->RemKeyItem(SecItemData);
    }
    else
    {
        // тут отправляем в бп объекта чтобы красиво поднять от стартовой позиции
        OnInteractUp(Interactor);
        bIsRaised = true;
        Interactor->AddKeyItem(OwnItemData);
    }

}

void ACombinationItem::OnInteractUp_Implementation(AMyBaseCharacter* Interactor)
{
     
}

void ACombinationItem::OnInteractDown_Implementation(AMyBaseCharacter* Interactor)
{

}

bool ACombinationItem::IsCorrectlyPositioned() const
{
    return GetActorLocation().Equals(CorrectLocation, 10.0f);
}

// Called every frame
void ACombinationItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}