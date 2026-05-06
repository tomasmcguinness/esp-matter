#include "mock_energy_evse_delegate.h"
#include "esp_log.h"

namespace chip {
namespace app {
namespace Clusters {
namespace EnergyEvse {

Protocols::InteractionModel::Status MockEnergyEVSEDelegate::Disable()
{
    ESP_LOGI(LOG_TAG, "%s is not implemented", __func__);
    return Protocols::InteractionModel::Status::Success;
}

Protocols::InteractionModel::Status MockEnergyEVSEDelegate::EnableCharging(const DataModel::Nullable<uint32_t>  &enableChargeTime, const int64_t  &minimumChargeCurrent,
                                                                           const int64_t  &maximumChargeCurrent)
{
    ESP_LOGI(LOG_TAG, "%s is not implemented", __func__);
    return Protocols::InteractionModel::Status::Success;
}

Protocols::InteractionModel::Status MockEnergyEVSEDelegate::EnableDischarging(const DataModel::Nullable<uint32_t>  &enableDischargeTime, const int64_t  &maximumDischargeCurrent)
{
    ESP_LOGI(LOG_TAG, "%s is not implemented", __func__);
    return Protocols::InteractionModel::Status::Success;
}

Protocols::InteractionModel::Status MockEnergyEVSEDelegate::StartDiagnostics()
{
    ESP_LOGI(LOG_TAG, "%s is not implemented", __func__);
    return Protocols::InteractionModel::Status::Success;
}

Protocols::InteractionModel::Status MockEnergyEVSEDelegate::SetTargets(const DataModel::DecodableList<Structs::ChargingTargetScheduleStruct::DecodableType>  &chargingTargetSchedules)
{
    ESP_LOGI(LOG_TAG, "%s is not implemented", __func__);
    return Protocols::InteractionModel::Status::Success;
}

Protocols::InteractionModel::Status MockEnergyEVSEDelegate::LoadTargets()
{
    ESP_LOGI(LOG_TAG, "%s is not implemented", __func__);
    return Protocols::InteractionModel::Status::Success;
}

Protocols::InteractionModel::Status MockEnergyEVSEDelegate::GetTargets(DataModel::List<const Structs::ChargingTargetScheduleStruct::Type>  &chargingTargetSchedules)
{
    ESP_LOGI(LOG_TAG, "%s is not implemented", __func__);
    return Protocols::InteractionModel::Status::Success;
}

Protocols::InteractionModel::Status MockEnergyEVSEDelegate::ClearTargets()
{
    ESP_LOGI(LOG_TAG, "%s is not implemented", __func__);
    return Protocols::InteractionModel::Status::Success;
}

void MockEnergyEVSEDelegate::OnStateChanged(StateEnum newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnSupplyStateChanged(SupplyStateEnum newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnFaultStateChanged(FaultStateEnum newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnChargingEnabledUntilChanged(DataModel::Nullable<uint32_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnDischargingEnabledUntilChanged(DataModel::Nullable<uint32_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnCircuitCapacityChanged(int64_t newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnMinimumChargeCurrentChanged(int64_t newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnMaximumChargeCurrentChanged(int64_t newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnMaximumDischargeCurrentChanged(int64_t newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnUserMaximumChargeCurrentChanged(int64_t newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnRandomizationDelayWindowChanged(uint32_t newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnNextChargeStartTimeChanged(DataModel::Nullable<uint32_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnNextChargeTargetTimeChanged(DataModel::Nullable<uint32_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnNextChargeRequiredEnergyChanged(DataModel::Nullable<int64_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnNextChargeTargetSoCChanged(DataModel::Nullable<Percent> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnApproximateEVEfficiencyChanged(DataModel::Nullable<uint16_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnStateOfChargeChanged(DataModel::Nullable<Percent> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnBatteryCapacityChanged(DataModel::Nullable<int64_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnVehicleIDChanged(DataModel::Nullable<CharSpan> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnSessionIDChanged(DataModel::Nullable<uint32_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnSessionDurationChanged(DataModel::Nullable<uint32_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnSessionEnergyChargedChanged(DataModel::Nullable<int64_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

void MockEnergyEVSEDelegate::OnSessionEnergyDischargedChanged(DataModel::Nullable<int64_t> newValue)
{
    ESP_LOGD(LOG_TAG, "%s", __func__);
    (void) newValue;
}

} // namespace EnergyEvse
} // namespace Clusters
} // namespace app
} // namespace chip
