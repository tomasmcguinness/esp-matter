/*
 *
 *    Copyright (c) 2025 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#include "camera-app.h"
#include "esp_matter_data_model_provider.h"

#include <app/SafeAttributePersistenceProvider.h>
#include <lib/support/CodeUtils.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::Chime;
using namespace chip::app::Clusters::WebRTCTransportProvider;
using namespace chip::app::Clusters::CameraAvStreamManagement;

static constexpr uint32_t kBitsPerMegabit = 1000000;

template <typename T>
using List   = chip::app::DataModel::List<T>;
using Status = Protocols::InteractionModel::Status;

CameraApp::CameraApp(chip::EndpointId aClustersEndpoint, CameraDeviceInterface * aCameraDevice)
{
    mEndpoint     = aClustersEndpoint;
    mCameraDevice = aCameraDevice;

    // Fetch all initialization parameters for CameraAVStreamMgmt Server
    BitFlags<CameraAvStreamManagement::Feature> avsmFeatures;
    BitFlags<CameraAvStreamManagement::OptionalAttribute> avsmOptionalAttrs;
    avsmFeatures.Set(CameraAvStreamManagement::Feature::kSnapshot);
    avsmFeatures.Set(CameraAvStreamManagement::Feature::kVideo);

    // Enable the Watermark and OSD features if camera supports
    if (mCameraDevice->GetCameraHALInterface().GetCameraSupportsWatermark()) {
        avsmFeatures.Set(CameraAvStreamManagement::Feature::kWatermark);
    }

    if (mCameraDevice->GetCameraHALInterface().GetCameraSupportsOSD()) {
        avsmFeatures.Set(CameraAvStreamManagement::Feature::kOnScreenDisplay);
    }

    if (mCameraDevice->GetCameraHALInterface().GetCameraSupportsSoftPrivacy()) {
        avsmFeatures.Set(CameraAvStreamManagement::Feature::kPrivacy);
    }

    // Check microphone support to set Audio feature
    if (mCameraDevice->GetCameraHALInterface().HasMicrophone()) {
        avsmFeatures.Set(CameraAvStreamManagement::Feature::kAudio);
        avsmOptionalAttrs.Set(CameraAvStreamManagement::OptionalAttribute::kMicrophoneAGCEnabled);
    }

    if (mCameraDevice->GetCameraHALInterface().HasLocalStorage()) {
        avsmFeatures.Set(CameraAvStreamManagement::Feature::kLocalStorage);
    }

    // Check if camera has speaker
    if (mCameraDevice->GetCameraHALInterface().HasSpeaker()) {
        avsmFeatures.Set(CameraAvStreamManagement::Feature::kSpeaker);
    }

    if (mCameraDevice->GetCameraHALInterface().GetCameraSupportsHDR()) {
        avsmFeatures.Set(CameraAvStreamManagement::Feature::kHighDynamicRange);
    }

    if (mCameraDevice->GetCameraHALInterface().GetCameraSupportsNightVision()) {
        avsmFeatures.Set(CameraAvStreamManagement::Feature::kNightVision);
        avsmOptionalAttrs.Set(CameraAvStreamManagement::OptionalAttribute::kNightVisionIllum);
    }

    if (mCameraDevice->GetCameraHALInterface().HasHardPrivacySwitch()) {
        avsmOptionalAttrs.Set(CameraAvStreamManagement::OptionalAttribute::kHardPrivacyModeOn);
    }

    if (mCameraDevice->GetCameraHALInterface().HasStatusLight()) {
        avsmOptionalAttrs.Set(CameraAvStreamManagement::OptionalAttribute::kStatusLightEnabled);
        avsmOptionalAttrs.Set(CameraAvStreamManagement::OptionalAttribute::kStatusLightBrightness);
    }

    if (mCameraDevice->GetCameraHALInterface().GetCameraSupportsImageControl()) {
        avsmFeatures.Set(CameraAvStreamManagement::Feature::kImageControl);
        avsmOptionalAttrs.Set(CameraAvStreamManagement::OptionalAttribute::kImageFlipVertical);
        avsmOptionalAttrs.Set(CameraAvStreamManagement::OptionalAttribute::kImageFlipHorizontal);
        avsmOptionalAttrs.Set(CameraAvStreamManagement::OptionalAttribute::kImageRotation);
    }

    uint32_t maxConcurrentVideoEncoders  = mCameraDevice->GetCameraHALInterface().GetMaxConcurrentEncoders();
    uint32_t maxEncodedPixelRate         = mCameraDevice->GetCameraHALInterface().GetMaxEncodedPixelRate();
    VideoSensorParamsStruct sensorParams = mCameraDevice->GetCameraHALInterface().GetVideoSensorParams();
    bool nightVisionUsesInfrared         = mCameraDevice->GetCameraHALInterface().GetNightVisionUsesInfrared();
    VideoResolutionStruct minViewport    = mCameraDevice->GetCameraHALInterface().GetMinViewport();
    std::vector<RateDistortionTradeOffStruct> rateDistortionTradeOffPoints =
        mCameraDevice->GetCameraHALInterface().GetRateDistortionTradeOffPoints();

    uint32_t maxContentBufferSize            = mCameraDevice->GetCameraHALInterface().GetMaxContentBufferSize();
    AudioCapabilitiesStruct micCapabilities  = mCameraDevice->GetCameraHALInterface().GetMicrophoneCapabilities();
    AudioCapabilitiesStruct spkrCapabilities = mCameraDevice->GetCameraHALInterface().GetSpeakerCapabilities();
    TwoWayTalkSupportTypeEnum twowayTalkSupport =
        mCameraDevice->GetCameraHALInterface().HasMicrophone() && mCameraDevice->GetCameraHALInterface().HasSpeaker()
        ? TwoWayTalkSupportTypeEnum::kFullDuplex
        : TwoWayTalkSupportTypeEnum::kNotSupported;
    std::vector<SnapshotCapabilitiesStruct> snapshotCapabilities = mCameraDevice->GetCameraHALInterface().GetSnapshotCapabilities();
    uint32_t maxNetworkBandwidth = mCameraDevice->GetCameraHALInterface().GetMaxNetworkBandwidth() * kBitsPerMegabit;
    std::vector<StreamUsageEnum> supportedStreamUsages = mCameraDevice->GetCameraHALInterface().GetSupportedStreamUsages();
    std::vector<StreamUsageEnum> streamUsagePriorities = mCameraDevice->GetCameraHALInterface().GetStreamUsagePriorities();

    // Instantiate the CameraAVStreamMgmt Server
    mAVStreamMgmtServerPtr = std::make_unique<CameraAVStreamManagementCluster>(
                                 CameraAVStreamManagementCluster::Context{ *app::GetSafeAttributePersistenceProvider() },
                                 mCameraDevice->GetCameraAVStreamMgmtDelegate(), mEndpoint, avsmFeatures, avsmOptionalAttrs,
                                 static_cast<uint8_t>(maxConcurrentVideoEncoders), maxEncodedPixelRate, sensorParams, nightVisionUsesInfrared, minViewport,
                                 rateDistortionTradeOffPoints, maxContentBufferSize, micCapabilities, spkrCapabilities, twowayTalkSupport,
                                 snapshotCapabilities, maxNetworkBandwidth, supportedStreamUsages, streamUsagePriorities);
}

CHIP_ERROR CameraApp::InitializeCameraAVStreamMgmt()
{
    VerifyOrReturnError(mAVStreamMgmtServerPtr != nullptr, CHIP_ERROR_INCORRECT_STATE);

    // Set the attribute defaults
    if (mCameraDevice->GetCameraHALInterface().GetCameraSupportsHDR()) {
        ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetHDRModeEnabled(mCameraDevice->GetCameraHALInterface().GetHDRMode()));
    }

    if (mCameraDevice->GetCameraHALInterface().GetCameraSupportsSoftPrivacy()) {
        ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetSoftRecordingPrivacyModeEnabled(
                                 mCameraDevice->GetCameraHALInterface().GetSoftRecordingPrivacyModeEnabled()));
        ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetSoftLivestreamPrivacyModeEnabled(
                                 mCameraDevice->GetCameraHALInterface().GetSoftLivestreamPrivacyModeEnabled()));
    }

    if (mCameraDevice->GetCameraHALInterface().HasHardPrivacySwitch()) {
        ReturnErrorOnFailure(
            mAVStreamMgmtServerPtr->SetHardPrivacyModeOn(mCameraDevice->GetCameraHALInterface().GetHardPrivacyMode()));
    }

    if (mCameraDevice->GetCameraHALInterface().GetCameraSupportsNightVision()) {
        ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetNightVision(mCameraDevice->GetCameraHALInterface().GetNightVision()));
    }

    ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetViewport(mCameraDevice->GetCameraHALInterface().GetViewport()));

    if (mCameraDevice->GetCameraHALInterface().HasSpeaker()) {
        ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetSpeakerMuted(mCameraDevice->GetCameraHALInterface().GetSpeakerMuted()));
        ReturnErrorOnFailure(
            mAVStreamMgmtServerPtr->SetSpeakerVolumeLevel(mCameraDevice->GetCameraHALInterface().GetSpeakerVolume()));
        ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetSpeakerMaxLevel(mCameraDevice->GetCameraHALInterface().GetSpeakerMaxLevel()));
        ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetSpeakerMinLevel(mCameraDevice->GetCameraHALInterface().GetSpeakerMinLevel()));
    }

    if (mCameraDevice->GetCameraHALInterface().HasMicrophone()) {
        ReturnErrorOnFailure(
            mAVStreamMgmtServerPtr->SetMicrophoneMuted(mCameraDevice->GetCameraHALInterface().GetMicrophoneMuted()));
        ReturnErrorOnFailure(
            mAVStreamMgmtServerPtr->SetMicrophoneVolumeLevel(mCameraDevice->GetCameraHALInterface().GetMicrophoneVolume()));
        ReturnErrorOnFailure(
            mAVStreamMgmtServerPtr->SetMicrophoneMaxLevel(mCameraDevice->GetCameraHALInterface().GetMicrophoneMaxLevel()));
        ReturnErrorOnFailure(
            mAVStreamMgmtServerPtr->SetMicrophoneMinLevel(mCameraDevice->GetCameraHALInterface().GetMicrophoneMinLevel()));
    }

    // Video and Snapshot features are already enabled.
    if (mCameraDevice->GetCameraHALInterface().HasLocalStorage()) {
        ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetLocalVideoRecordingEnabled(
                                 mCameraDevice->GetCameraHALInterface().GetLocalVideoRecordingEnabled()));
        ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetLocalSnapshotRecordingEnabled(
                                 mCameraDevice->GetCameraHALInterface().GetLocalSnapshotRecordingEnabled()));
    }

    if (mCameraDevice->GetCameraHALInterface().HasStatusLight()) {
        ReturnErrorOnFailure(
            mAVStreamMgmtServerPtr->SetStatusLightEnabled(mCameraDevice->GetCameraHALInterface().GetStatusLightEnabled()));
    }

    if (mCameraDevice->GetCameraHALInterface().GetCameraSupportsImageControl()) {
        ReturnErrorOnFailure(mAVStreamMgmtServerPtr->SetImageRotation(mCameraDevice->GetCameraHALInterface().GetImageRotation()));
        ReturnErrorOnFailure(
            mAVStreamMgmtServerPtr->SetImageFlipVertical(mCameraDevice->GetCameraHALInterface().GetImageFlipVertical()));
        ReturnErrorOnFailure(
            mAVStreamMgmtServerPtr->SetImageFlipHorizontal(mCameraDevice->GetCameraHALInterface().GetImageFlipHorizontal()));
    }

    return mAVStreamMgmtServerPtr->Init();
}

void CameraApp::InitCameraDeviceClusters()
{
    // Initialize Cluster Servers
    mWebRTCTransportProviderServer.Create(mEndpoint, mCameraDevice->GetWebRTCProviderDelegate());
    CHIP_ERROR err = esp_matter::data_model::provider::get_instance().registry().Register(mWebRTCTransportProviderServer.Registration());
    if (err != CHIP_NO_ERROR) {
        ChipLogError(Camera,
                     "Failed to register WebRTCTransportProvider on endpoint %u: "
                     "%" CHIP_ERROR_FORMAT,
                     mEndpoint, err.Format());
    }
    err = InitializeCameraAVStreamMgmt();
    if (err != CHIP_NO_ERROR) {
        ChipLogError(Camera, "Failed to initialize CameraAVStreamManagement on endpoint %u: %" CHIP_ERROR_FORMAT, mEndpoint,
                     err.Format());
    }
}

void CameraApp::ShutdownCameraDeviceClusters()
{
    ChipLogDetail(Camera, "CameraAppShutdown: Shutting down Camera device clusters");

    CHIP_ERROR err = esp_matter::data_model::provider::get_instance().registry().Unregister(&mWebRTCTransportProviderServer.Cluster());
    if (err != CHIP_NO_ERROR) {
        ChipLogError(Camera, "WebRTCTransportProvider unregister error: %" CHIP_ERROR_FORMAT, err.Format());
    }
    mWebRTCTransportProviderServer.Destroy();
}

static constexpr EndpointId kCameraEndpointId = 1;

std::unique_ptr<CameraApp> gCameraApp;

void CameraAppInit(CameraDeviceInterface * cameraDevice)
{

    gCameraApp = std::make_unique<CameraApp>(kCameraEndpointId, cameraDevice);

    gCameraApp.get()->InitCameraDeviceClusters();

    ChipLogDetail(Camera, "CameraAppInit: Initialized Camera clusters");
}

void CameraAppShutdown()
{
    ChipLogDetail(Camera, "CameraAppShutdown: Shutting down Camera app");
    gCameraApp.get()->ShutdownCameraDeviceClusters();
    gCameraApp = nullptr;
}
