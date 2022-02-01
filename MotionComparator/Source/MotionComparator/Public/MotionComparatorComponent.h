// Copyright 2022 Yacob N. S. Reyneke All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MotionComparatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), BlueprintType, meta=(BlueprintSpawnableComponent) )
class MOTIONCOMPARATOR_API UMotionComparatorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMotionComparatorComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Motion",
		meta=(ToolTip="The list of dynamically spawned motion point actors"))
	TArray<class AMotionPoint*> MotionPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup", AdvancedDisplay,
		meta=(ToolTip="The component that must follow this motion"))
	UPrimitiveComponent* ReferenceComponent;

	void Start();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup",
		meta=(ToolTip="+/- tolerance in degrees that compared component rotation must be within\nX=Roll, Y=Pitch, Z=Yaw\nSet to 0 or less to ignore axis"))
	FVector RotationTolerance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup",
		meta=(ToolTip="Material used on MotionPoint.Indicator to represent completed motion section"))
	class UMaterial* IndicatorMaterialMotionComplete;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup",
		meta=(ToolTip="Scale that the indicator for the next point in motion is set to\nUsed to highlight next point to user"))
	float NextIndicatorScale{ 1.25f };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup", AdvancedDisplay,
		meta=(ToolTip="The owning motion of this component"))
	class AMotion* ReferenceToMotion;

	// Flag determining if motion should still be checked - ensures ReferenceToMotion->OnMotionComplete/OnMotionFailed only triggered once
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup", AdvancedDisplay,
		meta=(ToolTip="When 'false', comparator is checking the reference component is complying with the motion"))
	bool bMotionEnd{ true };

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
