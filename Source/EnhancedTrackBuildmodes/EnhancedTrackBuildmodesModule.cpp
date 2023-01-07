#include "EnhancedTrackBuildmodesModule.h"
#include "EnhancedTrackBuildmodesLogger.h"


void FEnhancedTrackBuildmodesModule::StartupModule()
{
    UE_LOG(LogEnhancedTrackBuildmodes, Display, TEXT("Startup EnhancedTrackBuildmodes"));
}

IMPLEMENT_GAME_MODULE(FEnhancedTrackBuildmodesModule, EnhancedTrackBuildmodes);