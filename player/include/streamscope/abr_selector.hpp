#pragma once

#include "streamscope/hls_manifest.hpp"

#include <vector>

const Representation* selectRepresentation(
    const std::vector<Representation>& representations,
    double throughputMbps,
    double bufferSeconds,
    double safetyFactor = 0.80
);
