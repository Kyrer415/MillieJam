// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractableInterface.h"
#include "MySpectralPlane.generated.h"

UCLASS()
class KYRER_MILLIEJAM_API AMySpectralPlane : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySpectralPlane();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteracted();

protected:

	// Статик меш предмета(визуальная часть)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Item")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DoorRoot;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Реализация функции интефрейса. _Implementation обязателен дляв BlueprintNativeEvent!
	virtual void OnInteract_Implementation(class AMyBaseCharacter* Interactor) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
