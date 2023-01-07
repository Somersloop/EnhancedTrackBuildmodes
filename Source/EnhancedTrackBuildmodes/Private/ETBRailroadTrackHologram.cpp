#include "ETBRailroadTrackHologram.h"


AETBRailroadTrackHologram::AETBRailroadTrackHologram()
{
    this->mSnappedFirst = 0;
    this->mRotationXY = 0.0f;
    this->mRotationZ = 0.0f;
    this->mGradient = 0.0f;
}

void AETBRailroadTrackHologram::BeginPlay()
{
    if (HasAuthority())
    {
        this->mSnappedFirst = 0;
        this->mRotationXY = 0;
        this->mRotationZ = 0;
        this->mGradient = 0;
        SetBuildMode(this->mTrackBuildModeDefault);
    }
    Super::BeginPlay();
}

void AETBRailroadTrackHologram::OnBuildModeChanged()
{
    if (HasAuthority())
    {
        this->mRotationXY = 0;
        this->mRotationZ = 0;
        this->mGradient = 0;
    }
    Super::OnBuildModeChanged();
}

bool AETBRailroadTrackHologram::DoMultiStepPlacement(bool isInputFromARelease)
{
    if (HasAuthority() && mBuildStep == ESplineHologramBuildStep::SHBS_FindStart)
    {
        this->mSnappedFirst = GetSnappedConnectionComponents().Num();
    }
    return Super::DoMultiStepPlacement(isInputFromARelease);
}

void AETBRailroadTrackHologram::GetSupportedBuildModes_Implementation(TArray<TSubclassOf<UFGHologramBuildModeDescriptor>>& out_buildmodes) const
{
    Super::GetSupportedBuildModes_Implementation(out_buildmodes);
    if (!HasAuthority())
    {
        return;
    }
    out_buildmodes.AddUnique(this->mTrackBuildModeDefault);
    out_buildmodes.AddUnique(this->mTrackBuildModeStraight);
    out_buildmodes.AddUnique(this->mTrackBuildModeManual);
}

void AETBRailroadTrackHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
    Super::SetHologramLocationAndRotation(hitResult);  
    if (!(HasAuthority() && hitResult.bBlockingHit && GetSplineData().Num() >= 2 && !mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].Location.IsZero()))
    {
        return;
    }
    if (GetCurrentBuildMode() == this->mTrackBuildModeStraight && mSnappedFirst && mBuildStep == ESplineHologramBuildStep::SHBS_PlacePoleOrSnapEnding)
    {
        mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].Location = mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].Location.ProjectOnTo(mSplineData[0].LeaveTangent);
        mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].ArriveTangent = mSplineData[0].ArriveTangent;
        mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].LeaveTangent = mSplineData[0].LeaveTangent;
        UpdateSplineComponent();
    }
    else if (GetCurrentBuildMode() == this->mTrackBuildModeManual && mSnappedFirst == GetSnappedConnectionComponents().Num() && mBuildStep == ESplineHologramBuildStep::SHBS_PlacePoleOrSnapEnding)
    {
        bool shift = GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::LeftShift) || GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::RightShift);
        FVector tangente = mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].LeaveTangent;
        tangente.Z = 0;
        tangente = tangente.RotateAngleAxis(mRotationXY, tangente.ZAxisVector);
        if (shift)
        {
            mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].Location.Z = 0;
        }
        mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].ArriveTangent = tangente;
        mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].LeaveTangent = tangente;
        UpdateSplineComponent();

        if (shift)
        {
            mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].Location.Z = mSplineComponent->GetSplineLength() * this->mGradient;
        }
        tangente.Z = tangente.Size2D() * mRotationZ;
        mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].ArriveTangent = tangente;
        mSplineData[mSplineComponent->GetNumberOfSplinePoints() - 1].LeaveTangent = tangente;
        UpdateSplineComponent();
    }
}

void AETBRailroadTrackHologram::Scroll(int32 delta)
{
    Super::Scroll(delta);
    if (!HasAuthority())
    {
        return;
    }
    auto playerContoller = GetWorld()->GetFirstPlayerController();
    if (playerContoller && GetCurrentBuildMode() == this->mTrackBuildModeManual)
    {
        bool alt = playerContoller->IsInputKeyDown(EKeys::LeftAlt) || playerContoller->IsInputKeyDown(EKeys::RightAlt);
        bool shift = playerContoller->IsInputKeyDown(EKeys::LeftShift) || playerContoller->IsInputKeyDown(EKeys::RightShift);
        if (!alt && !shift)
        {
            this->mRotationXY += delta * 5.0f;
            if (this->mRotationXY > 175.0f)
            {
                this->mRotationXY = 175.0f;
            }
            else if (this->mRotationXY < -175.0f)
            {
                this->mRotationXY = -175.0f;
            }
            return;
        }
        if (alt)
        {
            this->mRotationZ += delta * 0.125f;
            if (this->mRotationZ > 0.5f)
            {
                this->mRotationZ = 0.5f;
            }
            else if (this->mRotationZ < -0.5f)
            {
                this->mRotationZ = -0.5f;
            }
        }
        if (shift)
        {
            this->mGradient += delta * 0.125f;
            if (this->mGradient > 0.5f)
            {
                this->mGradient = 0.5f;
            }
            else if (this->mGradient < -0.5f)
            {
                this->mGradient = -0.5f;
            }
        }
    }
}
