// Copyright 2022 Yacob N. S. Reyneke All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionPoint.generated.h"

UENUM()
enum PointStatus {
	Ready, // Not yet checked, not being checked
	Started, // Not yet completed, being checked
	Completed, // Motion successfully completed
	Failed // Motion failed to comply with required values
};

UCLASS()
class MOTIONCOMPARATOR_API AMotionPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMotionPoint();

	UPROPERTY(BlueprintReadOnly, Category = "Motion",
		meta=(ToolTip="The index of this motion point in the motion - i.e. 0 is 1st point"))
	int PointIndex{ 0 };

	UPROPERTY(BlueprintReadOnly, Category = "Motion",
		meta = (ToolTip = "Set to 'Completed' once this motion point is complete and the start of the next is reached"))
	TEnumAsByte<PointStatus> MotionStatus{ Ready };

	// Check if the comparison component is within the relevant collision box(es) of this motion point
	UFUNCTION(Category = "Motion")
	PointStatus VerifyPosition();

	// Check if the comparison component is within rotational tolerance of this motion point as set by designer
	UFUNCTION(Category = "Motion")
	PointStatus VerifyRotation(FVector RotationTolerance, FVector NextPointRelativeRotation = FVector());

	// MotonPoint initialization - required for comparison to work correctly - called during motion initialization step
	UFUNCTION(Category = "Motion")
	void Initialize(const int Index, UPrimitiveComponent* MotionReferenceComponent);

	bool VerifyAsEndPoint(FVector RotationTolerance);
	// Returns true if this point is the last point in the motion
	bool GetEndPoint() { return bLastPoint; }
	// Sets this point as the last point in the motion and removes the movement collision box
	void SetEndPoint();
	void UdpateIndicatorMaterial(class UMaterial* newMat);
	void SetIndicatorScale(bool defaultScale=true, float newScale=1.0f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// A collision box that represents the start position of this motion section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion")
	class UBoxComponent* PointCollisionBox;

	// A collision box that represents the movement section of this motion
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion")
	class UBoxComponent* MovementCollisionBox;

	// A mesh to show the start point and rotation of this motion to the user
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion")
	class UStaticMeshComponent* Indicator;

	// When 'false', detection algorithm ignores MovementCollisionBox
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Motion",
		meta = (ToolTip = "Set to False to disable MovementCollisionBox"))
	bool bHasMotion{ true };

	// The component that must follow this motion
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Monitor")
	UPrimitiveComponent* ReferenceComponent;

	// Whether or not the initialize function was called for this component
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Monitor")
	bool bInitialized{ false };

	// This variable is automatically set during initialization
	// When true - this point is treated as the last point and MovementCollisionBox is disabled
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Monitor")
	bool bLastPoint{ false };

	// This function sets whether the Indicator static mesh is visible
	// Visible: whether indicator static mesh should be set to visible or not
	UFUNCTION(BlueprintCallable)
	void SetIndicatorVisible(bool Visible);

	// Placeholder for the original scale of the indicator
	FVector baseIndicatorScale{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
