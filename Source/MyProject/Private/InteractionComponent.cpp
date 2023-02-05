#include "InteractionComponent.h"
#include "InteractionInterface.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SceneComponent.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsInteractionPressed = false;
	bIsInteractionCompleted = false;
}


void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* NewTarget = GetTarget();
	if (NewTarget != PreviousTarget)
		OnActiveTargetChanged(NewTarget);

	if (auto* FPC = GetWorld()->GetFirstPlayerController())
	{
		if (NewTarget)
		{
			float InteractionTime = IInteractionInterface::Execute_GetInteractionTime(NewTarget);
			if (InteractionTime > 0.f)
			{
				if (FPC->IsInputKeyDown(InteractionKey) && !bIsInteractionCompleted)
				{
					bIsInteractionPressed = true;
					float CurrentKeyTimeDown = FPC->GetInputKeyTimeDown(InteractionKey);
					IInteractionInterface::Execute_OnInteractionInProgress(NewTarget, CurrentKeyTimeDown);
					if (CurrentKeyTimeDown >= InteractionTime)
					{
						bIsInteractionCompleted = true;
						IInteractionInterface::Execute_OnInteraction(NewTarget);
					}
				}

				if (!FPC->IsInputKeyDown(InteractionKey))
				{
					bIsInteractionCompleted = false;
					if (bIsInteractionPressed)
					{
						bIsInteractionPressed = false;
						IInteractionInterface::Execute_OnInteractionProgressStoped(NewTarget);
					}
				}
			}
			else if (FPC->IsInputKeyDown(InteractionKey) && !bIsInteractionCompleted)
			{
				bIsInteractionCompleted = true;
				IInteractionInterface::Execute_OnInteraction(NewTarget);
			}
			else
			{
				bIsInteractionCompleted = false;
			}
		}
	}
}

AActor* UInteractionComponent::GetTarget()
{
	float SquaredAttackDistance = FMath::Square(InteractionDistance);
	float CurrentMinAngle = FLT_MAX;
	AActor* CurrentActor = nullptr;
	for (auto ItActor = TActorIterator<AActor>(GetWorld()); ItActor; ++ItActor)
	{
		if (*ItActor == GetOwner())
			continue;

		if (UKismetSystemLibrary::DoesImplementInterface(*ItActor, UInteractionInterface::StaticClass()))
		{
			const FVector DPSource = OverrideInteractionSource ? OverrideInteractionSource->GetForwardVector().GetSafeNormal() : GetOwner()->GetActorForwardVector().GetSafeNormal();
			const FVector DPLocation = OverrideInteractionSource ? OverrideInteractionSource->GetComponentLocation() : GetOwner()->GetActorLocation();
			float AngleBtwTargetAndPlayer = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(DPSource, (ItActor->GetActorLocation() - DPLocation).GetSafeNormal())));
			float CurrentDistance = ItActor->GetSquaredDistanceTo(GetOwner());
			if (CurrentDistance <= SquaredAttackDistance && CurrentMinAngle > AngleBtwTargetAndPlayer && InteractionAngle >= AngleBtwTargetAndPlayer)
			{
				CurrentMinAngle = AngleBtwTargetAndPlayer;
				CurrentActor = *ItActor;
			}
		}
	}

	return CurrentActor;
}

void UInteractionComponent::OnActiveTargetChanged(AActor* NewTarget)
{

	if (NewTarget)
		IInteractionInterface::Execute_OnBecameActive(NewTarget);

	if (PreviousTarget)
		IInteractionInterface::Execute_OnStoppedActive(PreviousTarget);

	PreviousTarget = NewTarget;
}
