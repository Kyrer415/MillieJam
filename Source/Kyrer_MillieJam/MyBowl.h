// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "MyInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "MyBowl.generated.h"

UCLASS()
class KYRER_MILLIEJAM_API AMyBowl : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBowl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Это событие вызовим в c++, и настроим внутри блупринта GameMode	
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteracted();

	// Статик меш предмета(визуальная часть)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Item")
	class UStaticMeshComponent* MeshComponent;

public:	

	virtual void OnInteract_Implementation(class AMyBaseCharacter* Interactor) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
