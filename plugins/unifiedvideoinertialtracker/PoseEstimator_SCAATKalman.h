/** @file
    @brief Header

    @date 2016

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2016 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef INCLUDED_PoseEstimator_SCAATKalman_h_GUID_F1FC2154_E59B_4598_70C2_253F8EA31485
#define INCLUDED_PoseEstimator_SCAATKalman_h_GUID_F1FC2154_E59B_4598_70C2_253F8EA31485

// Internal Includes
#include "ConfigParams.h"
#include "PoseEstimatorTypes.h"
#include "ModelTypes.h"
#include "TrackedBodyTarget.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace vbtracker {

    class SCAATKalmanPoseEstimator {
      public:
        enum class TriBool { False, True, Unknown };
        enum class TrackingHealth {
            Functioning,
            NeedsResetNow,
            ResetWhenBeaconsSeen
        };
#if 0
        struct InOutParams {
            BeaconStateVec &beacons;
            std::vector<double> const &beaconMeasurementVariance;
            std::vector<bool> const &beaconFixed;
            Vec3Vector const &beaconEmissionDirection;
            BodyState &state;
            BodyProcessModel &processModel;
            std::vector<BeaconData> & beaconDebug;
        };
#endif
        SCAATKalmanPoseEstimator(ConfigParams const &params);
        bool operator()(CameraParameters const &camParams,
                        LedPtrList const &leds,
                        osvr::util::time::TimeValue const &frameTime,
                        double videoDt, EstimatorInOutParams const &p);

        void resetCounters() {
            m_framesInProbation = 0;
            m_framesWithoutIdentifiedBlobs = 0;
            m_framesWithoutUtilizedMeasurements = 0;
        }

        /// Determines whether the Kalman filter is in good working condition,
        /// should fall back to RANSAC immediately, or should fall back next
        /// time beacons are detected. When the algorithm switches f
        TrackingHealth getTrackingHealth();

      private:
        TriBool inBoundingBoxRatioRange(Led const &led);
        float m_maxBoxRatio;
        float m_minBoxRatio;
        const bool m_shouldSkipBright;
        const double m_maxSquaredResidual;
        const double m_maxZComponent;
        const double m_highResidualVariancePenalty;
        const double m_beaconProcessNoise;
        const double m_measurementVarianceScaleFactor;
        const bool m_extraVerbose;
        std::size_t m_framesInProbation = 0;
        std::size_t m_framesWithoutIdentifiedBlobs = 0;
        std::size_t m_framesWithoutUtilizedMeasurements = 0;
    };
} // namespace vbtracker
} // namespace osvr

#endif // INCLUDED_PoseEstimator_SCAATKalman_h_GUID_F1FC2154_E59B_4598_70C2_253F8EA31485
