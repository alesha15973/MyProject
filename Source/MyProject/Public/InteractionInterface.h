// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API IInteractionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	float GetInteractionTime();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void OnBecameActive();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void OnStoppedActive();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void OnInteraction();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void OnInteractionInProgress(float InteractionProgress);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void OnInteractionProgressStoped();

};
