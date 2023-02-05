#pragma once
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "InteractionComponent.generated.h"

class IInteractionInterface;
class USceneComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionAngle = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FKey InteractionKey = EKeys::E;

	UFUNCTION(BlueprintCallable)
	void SetNewInteractionSource(USceneComponent* NewInteractionSource) { OverrideInteractionSource = NewInteractionSource; }

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnActiveTargetChanged(AActor* NewTarget);

	AActor* GetTarget();

	UPROPERTY()
	AActor* PreviousTarget;

	UPROPERTY()
	USceneComponent* OverrideInteractionSource;

	uint8 bIsInteractionPressed : 1;
	uint8 bIsInteractionCompleted : 1;
};


