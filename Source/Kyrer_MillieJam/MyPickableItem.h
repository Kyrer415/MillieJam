// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBaseCharacter.h" // Нужен ради структуры FInventoryItem
#include "MyInteractableInterface.h"
#include "MyPickableItem.generated.h"

UCLASS()
class KYRER_MILLIEJAM_API AMyPickableItem : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPickableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Статик меш предмета(визуальная часть)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Item")
	class UStaticMeshComponent* MeshComponent;

	// Флажок: еда это или квестовый предмет?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Item")
	bool bIsFood = false;

	// Данные предмета (будут использоваться, если bIsFood == false)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Item", meta = (EditCondition = "!bIsFood"))
	FInventoryItem ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings", meta = (EditCondition = "bIsFood"))
	int32 FoodAmount = 1;

public:	
	// Реализация функции интефрейса. _Implementation обязателен дляв BlueprintNativeEvent!
	virtual void OnInteract_Implementation(class AMyBaseCharacter* Interactor) override;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
