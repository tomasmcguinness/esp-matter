/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "esp_log.h"

#include "mock_webrtc_transport_provider_delegate.h"

namespace chip {
namespace app {
namespace Clusters {
namespace WebRTCTransportProvider {

CHIP_ERROR MockWebRTCTransportProviderDelegate::HandleSolicitOffer(const OfferRequestArgs  &args, WebRTCSessionStruct  &outSession,
                                                                   bool  &outDeferredOffer)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) args;
    (void) outSession;
    outDeferredOffer = false;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::HandleProvideOffer(const ProvideOfferRequestArgs  &args, WebRTCSessionStruct  &outSession)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) args;
    (void) outSession;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::HandleProvideAnswer(uint16_t sessionId, const std::string  &sdpAnswer)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) sessionId;
    (void) sdpAnswer;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::HandleProvideICECandidates(uint16_t sessionId,
                                                                           const std::vector<ICECandidateStruct>  &candidates)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) sessionId;
    (void) candidates;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::HandleEndSession(uint16_t sessionId, WebRTCEndReasonEnum reasonCode)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) sessionId;
    (void) reasonCode;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::ValidateStreamUsage(StreamUsageEnum streamUsage,
                                                                    Optional<std::vector<uint16_t>>  &videoStreams,
                                                                    Optional<std::vector<uint16_t>>  &audioStreams)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) streamUsage;
    (void) videoStreams;
    (void) audioStreams;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::ValidateVideoStreamID(uint16_t videoStreamId)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) videoStreamId;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::ValidateAudioStreamID(uint16_t audioStreamId)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) audioStreamId;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::ValidateVideoStreams(const std::vector<uint16_t>  &videoStreams)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) videoStreams;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::ValidateAudioStreams(const std::vector<uint16_t>  &audioStreams)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) audioStreams;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::IsStreamUsageSupported(Globals::StreamUsageEnum streamUsage)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) streamUsage;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::IsHardPrivacyModeActive(bool  &isActive)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    isActive = false;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::IsSoftRecordingPrivacyModeActive(bool  &isActive)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    isActive = false;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::IsSoftLivestreamPrivacyModeActive(bool  &isActive)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    isActive = false;
    return CHIP_NO_ERROR;
}

bool MockWebRTCTransportProviderDelegate::HasAllocatedVideoStreams()
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    return true;
}

bool MockWebRTCTransportProviderDelegate::HasAllocatedAudioStreams()
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    return true;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::ValidateSFrameConfig(uint16_t cipherSuite, size_t baseKeyLength)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    (void) cipherSuite;
    (void) baseKeyLength;
    return CHIP_NO_ERROR;
}

CHIP_ERROR MockWebRTCTransportProviderDelegate::IsUTCTimeNull(bool  &isNull)
{
    ESP_LOGE(LOG_TAG, "%s is not implemented", __func__);
    isNull = false;
    return CHIP_NO_ERROR;
}

} // namespace WebRTCTransportProvider
} // namespace Clusters
} // namespace app
} // namespace chip
