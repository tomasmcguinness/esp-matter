// Copyright 2025 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <esp_matter_data_model_priv.h>

#include <app/AttributeAccessInterfaceRegistry.h>
#include <app/ClusterCallbacks.h>
#include <app/clusters/power-source-server/power-source-server.h>
#include <app/util/attribute-storage.h>
#include <app/util/config.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <clusters/PowerSource/Metadata.h>
#include <data_model/esp_matter_data_model.h>
#include <cstring>
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>

// NOTE: Upstream PowerSource has CodegenIntegration but uses AAI (not SCI). Since esp-matter excludes upstream
// CodegenIntegration in favor of local integration, this file implements MatterPowerSource callbacks to keep
// generated-data-model behavior consistent and avoid all_device_types_app link errors (PluginServerInitCallback).
// Update this integration to SCI in a future submodule update.

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::PowerSource::Attributes;

namespace {

struct PowerSourceClusterInfo {
    PowerSourceClusterInfo() : mClusterEndpoint(kInvalidEndpointId) {}
    explicit PowerSourceClusterInfo(EndpointId powerClusterEndpointId) : mClusterEndpoint(powerClusterEndpointId) {}
    void Clear()
    {
        mBuf.Free();
        mEndpointList = Span<EndpointId>();
    }
    CHIP_ERROR SetEndpointList(Span<EndpointId> endpointList)
    {
        Clear();
        if (endpointList.size() == 0) {
            mEndpointList = Span<EndpointId>();
            return CHIP_NO_ERROR;
        }
        mBuf.Calloc(endpointList.size());
        if (mBuf.Get() == nullptr) {
            return CHIP_ERROR_NO_MEMORY;
        }
        memcpy(mBuf.Get(), endpointList.data(), endpointList.size() * sizeof(EndpointId));
        mEndpointList = Span<EndpointId>(mBuf.Get(), endpointList.size());
        return CHIP_NO_ERROR;
    }
    EndpointId mClusterEndpoint = kInvalidEndpointId;
    Platform::ScopedMemoryBuffer<EndpointId> mBuf;
    Span<EndpointId> mEndpointList;
};

PowerSourceServer gPowerSourceServer;
PowerSourceAttrAccess gAttrAccess;

static constexpr uint16_t kNumStaticEndpoints = MATTER_DM_POWER_SOURCE_CLUSTER_SERVER_ENDPOINT_COUNT;
#define POWER_SERVER_NUM_SUPPORTED_ENDPOINTS (MATTER_DM_POWER_SOURCE_CLUSTER_SERVER_ENDPOINT_COUNT + CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
static constexpr size_t kNumSupportedEndpoints = POWER_SERVER_NUM_SUPPORTED_ENDPOINTS;

#if POWER_SERVER_NUM_SUPPORTED_ENDPOINTS > 0
PowerSourceClusterInfo sPowerSourceClusterInfo[kNumSupportedEndpoints] = {};
#else
PowerSourceClusterInfo * sPowerSourceClusterInfo = nullptr;
#endif

} // namespace

void ESPMatterPowerSourceClusterServerInitCallback(EndpointId /* endpointId */)
{
    // Attribute access is registered once from ``MatterPowerSourcePluginServerInitCallback`` (CALL_ONCE).
}

void ESPMatterPowerSourceClusterServerShutdownCallback(EndpointId endpointId, chip::app::ClusterShutdownType /* shutdownType */)
{
    // Clear cached EndpointList for this Power Source cluster instance only (matches per-endpoint teardown semantics).
    (void) PowerSourceServer::Instance().SetEndpointList(endpointId, Span<EndpointId>());
}

void MatterPowerSourcePluginServerInitCallback()
{
    AttributeAccessInterfaceRegistry::Instance().Register(&gAttrAccess);
}

void MatterPowerSourcePluginServerShutdownCallback()
{
    AttributeAccessInterfaceRegistry::Instance().Unregister(&gAttrAccess);
}

namespace chip {
namespace app {
namespace Clusters {

CHIP_ERROR PowerSourceAttrAccess::Read(const ConcreteReadAttributePath  &aPath, AttributeValueEncoder  &aEncoder)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    switch (aPath.mAttributeId) {
    case ActiveBatFaults::Id:
        err = aEncoder.EncodeEmptyList();
        break;
    case EndpointList::Id: {
        PowerSourceServer  &server    = PowerSourceServer::Instance();
        const Span<EndpointId> * span = server.GetEndpointList(aPath.mEndpointId);
        if (span == nullptr) {
            err = aEncoder.EncodeEmptyList();
        } else {
            err = aEncoder.EncodeList([span](const auto & encoder) -> CHIP_ERROR {
                for (auto id : *span) {
                    ReturnErrorOnFailure(encoder.Encode(id));
                }
                return CHIP_NO_ERROR;
            });
        }
        break;
    }
    case ClusterRevision::Id:
        err = aEncoder.Encode(PowerSource::kRevision);
        break;
    default:
        break;
    }

    return err;
}

PowerSourceAttrAccess  &TestOnlyGetPowerSourceAttrAccess()
{
    return gAttrAccess;
}

PowerSourceServer  &PowerSourceServer::Instance()
{
    return gPowerSourceServer;
}

CHIP_ERROR PowerSourceServer::SetEndpointList(EndpointId powerSourceClusterEndpoint, Span<EndpointId> endpointList)
{
    uint16_t idx = emberAfGetClusterServerEndpointIndex(powerSourceClusterEndpoint, Clusters::PowerSource::Id, kNumStaticEndpoints);
    if (idx == kEmberInvalidEndpointIndex) {
        return CHIP_ERROR_NO_MEMORY;
    }

    sPowerSourceClusterInfo[idx].Clear();
    if (endpointList.size() == 0) {
        sPowerSourceClusterInfo[idx] = PowerSourceClusterInfo();
        return CHIP_NO_ERROR;
    }

    sPowerSourceClusterInfo[idx] = PowerSourceClusterInfo(powerSourceClusterEndpoint);
    return sPowerSourceClusterInfo[idx].SetEndpointList(endpointList);
}

const Span<EndpointId> * PowerSourceServer::GetEndpointList(EndpointId powerSourceClusterEndpoint) const
{
    uint16_t idx = emberAfGetClusterServerEndpointIndex(powerSourceClusterEndpoint, Clusters::PowerSource::Id, kNumStaticEndpoints);
    if (idx != kEmberInvalidEndpointIndex && sPowerSourceClusterInfo[idx].mEndpointList.size() > 0) {
        return &sPowerSourceClusterInfo[idx].mEndpointList;
    }
    return nullptr;
}

void PowerSourceServer::Shutdown()
{
#if POWER_SERVER_NUM_SUPPORTED_ENDPOINTS > 0
    for (size_t i = 0; i < kNumSupportedEndpoints; ++i) {
        sPowerSourceClusterInfo[i].Clear();
    }
#endif
}

size_t PowerSourceServer::GetNumSupportedEndpointLists() const
{
    return kNumSupportedEndpoints;
}

} // namespace Clusters
} // namespace app
} // namespace chip
