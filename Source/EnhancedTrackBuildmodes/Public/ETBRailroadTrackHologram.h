

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGRailroadTrackHologram.h"
#include "ETBRailroadTrackHologram.generated.h"

/**
 * 
 */
UCLASS()
class ENHANCEDTRACKBUILDMODES_API AETBRailroadTrackHologram : public AFGRailroadTrackHologram
{
	GENERATED_BODY()

public:

    AETBRailroadTrackHologram();

    virtual bool DoMultiStepPlacement(bool isInputFromARelease) override;

    virtual void BeginPlay() override;
    virtual void OnBuildModeChanged() override;
    virtual void GetSupportedBuildModes_Implementation(TArray<TSubclassOf<UFGHologramBuildModeDescriptor>>& out_buildmodes) const override;
    virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;
    virtual void Scroll(int32 delta) override;



    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Track BuildModes")
        TSubclassOf<UFGHologramBuildModeDescriptor> mTrackBuildModeDefault;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Track BuildModes")
        TSubclassOf<UFGHologramBuildModeDescriptor> mTrackBuildModeManual;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Track BuildModes")
        TSubclassOf<UFGHologramBuildModeDescriptor> mTrackBuildModeStraight;

protected:
    int32 mSnappedFirst;
    float mRotationXY;
    float mRotationZ;
    float mGradient;
};
