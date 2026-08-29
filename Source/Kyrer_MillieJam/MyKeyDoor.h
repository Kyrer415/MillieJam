// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractableInterface.h"
#include "MyKeyDoor.generated.h"

UCLASS()
class KYRER_MILLIEJAM_API AMyKeyDoor : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyKeyDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "MillieJam|Door")
	void OpenDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "MillieJam|Door")
	void NoKey();

	UPROPERTY(EditAnywhere, Category = "Lock")
	FName RequiredKeyID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Статик меш предмета(визуальная часть)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Door")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MillieJam|Components")
	USceneComponent* DoorRoot;
	// Реализация функции интефрейса. _Implementation обязателен дляв BlueprintNativeEvent!
	virtual void OnInteract_Implementation(class AMyBaseCharacter* Interactor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
