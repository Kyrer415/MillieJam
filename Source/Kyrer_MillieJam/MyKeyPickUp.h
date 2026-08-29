// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractableInterface.h"
#include "MyBaseCharacter.h"
#include "MyKeyPickUp.generated.h"

UCLASS()
class KYRER_MILLIEJAM_API AMyKeyPickUp : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyKeyPickUp();

protected:

	UPROPERTY(VisibleAnywhere, Category = "MillieJam|Components")
	class UStaticMeshComponent* KeyMesh;

	// уникальный ID предмета для HasItem
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Key")
	FInventoryItem KeyItemData;

	virtual void OnInteract_Implementation(AMyBaseCharacter* Interactor);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
