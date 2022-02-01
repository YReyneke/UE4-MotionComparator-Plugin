// Copyright 2022 Yacob N. S. Reyneke All Rights Reserved.


#include "MotionComparatorComponent.h"
#include "MotionPoint.h"
#include "Motion.h"

// Sets default values for this component's properties
UMotionComparatorComponent::UMotionComparatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMotionComparatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ReferenceToMotion = Cast<AMotion>(GetOwner());
	if (ReferenceToMotion == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("MotionComparatorComponent.Constructor(): Owner is not of type AMotion!"));
		bMotionEnd = true;
	}
}


// Called every frame
void UMotionComparatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (!bMotionEnd) {
		// FailSafe!
		if (!MotionPoints[MotionPoints.Num() - 1]->GetEndPoint()) {
			UE_LOG(LogTemp, Warning, TEXT("MotionComparatorComponent.BeginPlay(): Last motion point detected automatically..."));
			MotionPoints[MotionPoints.Num() - 1]->SetEndPoint();
		}
		for (int i{ 0 }; i < MotionPoints.Num(); i++) {
			if (MotionPoints[i]->MotionStatus == Completed) {
				if (i == MotionPoints.Num() - 1) {
					ReferenceToMotion->OnMotionComplete();
					bMotionEnd = true;
				}
				continue; // Move to next motion point
			}
			if (MotionPoints[i]->MotionStatus == Failed) {
				ReferenceToMotion->OnMotionFailed();
				bMotionEnd = true;
				break; // Exit for loop
			}
			if (MotionPoints[i]->MotionStatus == Started) {
				// Verify Component position is in tolerance
				MotionPoints[i]->VerifyPosition();

				// Verify Component rotation is in tolerance
				if (MotionPoints[i]->GetEndPoint()) {
					MotionPoints[i]->VerifyRotation(RotationTolerance);
				}
				else {
					FVector NextPointRelativeRotation = MotionPoints[i + 1]->GetActorRotation().Euler();
					NextPointRelativeRotation -= MotionPoints[i]->GetActorRotation().Euler();
					MotionPoints[i]->VerifyRotation(RotationTolerance, NextPointRelativeRotation);
				}

				// Update status to complete if ReferenceComponent within limits of next point
				// Or final point if all other motions are complete
				if (i < MotionPoints.Num() - 1) {
					if (MotionPoints[i + 1]->VerifyAsEndPoint(RotationTolerance)) {
						MotionPoints[i]->MotionStatus = Completed;
						MotionPoints[i + 1]->SetIndicatorScale(); // Reset to default scale
					}
				}
				else {
					if (MotionPoints[i]->VerifyAsEndPoint(RotationTolerance)) {
						MotionPoints[i]->MotionStatus = Completed;
						MotionPoints[i]->UdpateIndicatorMaterial(IndicatorMaterialMotionComplete);
						MotionPoints[i]->SetIndicatorScale(); // Reset to default scale
					}
				}
				break; // Exit for loop
			}
			if (MotionPoints[i]->MotionStatus == Ready) { // Will only reach here if previous point status is Complete - NOTE: Initialize() sets first point to 'Started'
				MotionPoints[i]->MotionStatus = Started; // Set this point as test motion point
				MotionPoints[i]->UdpateIndicatorMaterial(IndicatorMaterialMotionComplete);// Indicate that previous motion is complete
				if (i < MotionPoints.Num() - 1) {
					MotionPoints[i + 1]->SetIndicatorScale(false, NextIndicatorScale); // Highlight next point to user
				}
				break; // Exit for loop
			}
		}
	}
}

void UMotionComparatorComponent::Start() {
	if (MotionPoints.Num() < 2) {
		UE_LOG(LogTemp, Error, TEXT("MotionComparatorComponent.Start: Not enough motion points in motion list!"));
		return;
	}
	MotionPoints[0]->UdpateIndicatorMaterial(IndicatorMaterialMotionComplete);
	MotionPoints[1]->SetIndicatorScale(false, NextIndicatorScale);
	bMotionEnd = false;
}