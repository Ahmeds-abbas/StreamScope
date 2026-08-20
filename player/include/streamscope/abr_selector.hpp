#pragma once

#include "streamscope/hls_manifest.hpp"

#include <vector>

const Representation* selectRepresentation(
    const std::vector<Representation>& representations,
    double throughputMbps,
    double safetyFactor = 0.80
);
