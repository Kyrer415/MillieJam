// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySectionalDoor.generated.h"

UCLASS()
class KYRER_MILLIEJAM_API AMySectionalDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySectionalDoor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DoorSettings|ID")
	int32 DoorID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DoorSettings|ID")
	int32 RequiredKeyID;

	UFUNCTION(BlueprintImplementableEvent)
	void OpenDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* DoorRoot;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* DoorMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool bShouldOpen = false;
};
