/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#pragma once

#include <app/clusters/webrtc-transport-provider-server/WebRTCTransportProviderCluster.h>

/*
 * Mock WebRTCTransportProvider Delegate Implementation
 * Stubs the Matter WebRTCTransportProvider::Delegate interface (see WebRTCTransportProviderCluster.h).
 */

namespace chip {
namespace app {
namespace Clusters {
namespace WebRTCTransportProvider {

class MockWebRTCTransportProviderDelegate : public Delegate {
public:
    MockWebRTCTransportProviderDelegate() = default;
    virtual ~MockWebRTCTransportProviderDelegate() = default;

    CHIP_ERROR HandleSolicitOffer(const OfferRequestArgs  &args, WebRTCSessionStruct  &outSession, bool  &outDeferredOffer) override;

    CHIP_ERROR HandleProvideOffer(const ProvideOfferRequestArgs  &args, WebRTCSessionStruct  &outSession) override;

    CHIP_ERROR HandleProvideAnswer(uint16_t sessionId, const std::string  &sdpAnswer) override;

    CHIP_ERROR HandleProvideICECandidates(uint16_t sessionId, const std::vector<ICECandidateStruct>  &candidates) override;

    CHIP_ERROR HandleEndSession(uint16_t sessionId, WebRTCEndReasonEnum reasonCode) override;

    CHIP_ERROR ValidateStreamUsage(StreamUsageEnum streamUsage, Optional<std::vector<uint16_t>>  &videoStreams,
                                   Optional<std::vector<uint16_t>>  &audioStreams) override;

    CHIP_ERROR ValidateVideoStreamID(uint16_t videoStreamId) override;

    CHIP_ERROR ValidateAudioStreamID(uint16_t audioStreamId) override;

    CHIP_ERROR ValidateVideoStreams(const std::vector<uint16_t>  &videoStreams) override;

    CHIP_ERROR ValidateAudioStreams(const std::vector<uint16_t>  &audioStreams) override;

    CHIP_ERROR IsStreamUsageSupported(Globals::StreamUsageEnum streamUsage) override;

    CHIP_ERROR IsHardPrivacyModeActive(bool  &isActive) override;

    CHIP_ERROR IsSoftRecordingPrivacyModeActive(bool  &isActive) override;

    CHIP_ERROR IsSoftLivestreamPrivacyModeActive(bool  &isActive) override;

    bool HasAllocatedVideoStreams() override;

    bool HasAllocatedAudioStreams() override;

    CHIP_ERROR ValidateSFrameConfig(uint16_t cipherSuite, size_t baseKeyLength) override;

    CHIP_ERROR IsUTCTimeNull(bool  &isNull) override;

private:
    static constexpr const char * LOG_TAG = "MockWebRTCTransportProviderDelegate";
};

} // namespace WebRTCTransportProvider
} // namespace Clusters
} // namespace app
} // namespace chip
