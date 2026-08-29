// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractableInterface.h"
#include "InteractableDoor.generated.h"

UCLASS()
class KYRER_MILLIEJAM_API AInteractableDoor : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "MillieJam|Interaction")
	void OnInteracted();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Статик меш предмета(визуальная часть)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|Item")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MillieJam|Components")
	USceneComponent* DoorRoot;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Реализация функции интефрейса. _Implementation обязателен дляв BlueprintNativeEvent!
	virtual void OnInteract_Implementation(class AMyBaseCharacter* Interactor) override;

};
