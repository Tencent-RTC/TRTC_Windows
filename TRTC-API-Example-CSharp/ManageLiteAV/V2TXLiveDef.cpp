// C++/CLI
#include "cli/Live/V2TXLiveDef.h"

#include "include/Live2/V2TXLiveDef.hpp"
#include "Utils.h"

namespace ManageLiteAV {
    V2TXLiveVideoEncoderParam::V2TXLiveVideoEncoderParam(V2TXLiveVideoResolution resolution) {
        liteav::V2TXLiveVideoResolution resolution_ = static_cast<liteav::V2TXLiveVideoResolution>(resolution);
        liteav::V2TXLiveVideoEncoderParam encoder_param(resolution_);
        videoResolution = resolution;
        videoResolutionMode = static_cast<ManageLiteAV::V2TXLiveVideoResolutionMode>(encoder_param.videoResolutionMode);
        videoFps = encoder_param.videoFps;
        videoBitrate = encoder_param.videoBitrate;
        minVideoBitrate = encoder_param.minVideoBitrate;
    }
}