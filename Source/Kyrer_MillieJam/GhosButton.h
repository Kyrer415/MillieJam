// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractableInterface.h"
#include "GhosButton.generated.h"

UCLASS()
class KYRER_MILLIEJAM_API AGhosButton : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhosButton();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteracted();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|ButtonID")
	int32 KeyID;

protected:

	// Статик меш предмета(визуальная часть)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Item")
	class UStaticMeshComponent* MeshComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Реализация функции интефрейса. _Implementation обязателен дляв BlueprintNativeEvent!
	virtual void OnInteract_Implementation(class AMyBaseCharacter* Interactor) override;
};
