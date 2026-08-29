// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBaseCharacter.h" 
#include <array>
#include "MyInteractableInterface.h"
#include "CombinationItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectInteracted, ACombinationItem*, InteractedObject);

UCLASS()
class KYRER_MILLIEJAM_API ACombinationItem : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombinationItem();

	// уникальный ID предмета для HasItem
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Item")
	FInventoryItem OwnItemData;

	// уникальный ID предмета для HasItem
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Item")
	FInventoryItem FirItemData;

	// уникальный ID предмета для HasItem
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Item")
	FInventoryItem SecItemData;

	// отчка в котороый предмет считается правильно установленным
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|Puzzle")
	FVector CorrectLocation;

	// награда за правильную комбинацию
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|Puzzle")
	int32 PuzzleKeyID = 0;

	UPROPERTY(BlueprintAssignable, Category = "MillieJam|Puzzle")
	FOnObjectInteracted OnObjectInteracted;

	UFUNCTION(BlueprintNativeEvent)
	void OnInteractDown(AMyBaseCharacter* Interactor);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnInteractUp(AMyBaseCharacter* Interactor);

	virtual void OnInteract_Implementation(AMyBaseCharacter* Interactor);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Puzzle")
	FVector StartPosition;

	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	void UpdateStartPosition(const FVector& NewStart) { StartPosition = NewStart; }

	// проверка положения
	UFUNCTION(BlueprintPure, Category = "Puzzle")
	bool IsCorrectlyPositioned() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle")
	bool bIsRaised = false;

	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	void SetRaised(bool bNewState) { bIsRaised = bNewState; }

	UFUNCTION(BlueprintPure, Category = "Puzzle")
	bool IsRaised() const { return bIsRaised; }

protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> ItemRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
