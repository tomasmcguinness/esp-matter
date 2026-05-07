/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#pragma once

#include <app/clusters/energy-evse-server/EnergyEvseDelegate.h>

/*
 * Mock EnergyEVSE Delegate Implementation
 * This file provides a mock implementation of the EnergyEvse::Delegate interface
 * that returns success for all command methods and no-ops attribute change callbacks.
 * See EnergyEvseDelegate.h in the Matter SDK.
 */

namespace chip {
namespace app {
namespace Clusters {
namespace EnergyEvse {

class MockEnergyEVSEDelegate : public Delegate {
public:
    MockEnergyEVSEDelegate() = default;

    Protocols::InteractionModel::Status Disable() override;

    Protocols::InteractionModel::Status EnableCharging(const DataModel::Nullable<uint32_t>  &enableChargeTime,
                                                       const int64_t  &minimumChargeCurrent,
                                                       const int64_t  &maximumChargeCurrent) override;

    Protocols::InteractionModel::Status EnableDischarging(const DataModel::Nullable<uint32_t>  &enableDischargeTime,
                                                          const int64_t  &maximumDischargeCurrent) override;

    Protocols::InteractionModel::Status StartDiagnostics() override;

    Protocols::InteractionModel::Status
    SetTargets(const DataModel::DecodableList<Structs::ChargingTargetScheduleStruct::DecodableType>  &chargingTargetSchedules) override;

    Protocols::InteractionModel::Status LoadTargets() override;

    Protocols::InteractionModel::Status
    GetTargets(DataModel::List<const Structs::ChargingTargetScheduleStruct::Type>  &chargingTargetSchedules) override;

    Protocols::InteractionModel::Status ClearTargets() override;

    void OnStateChanged(StateEnum newValue) override;
    void OnSupplyStateChanged(SupplyStateEnum newValue) override;
    void OnFaultStateChanged(FaultStateEnum newValue) override;
    void OnChargingEnabledUntilChanged(DataModel::Nullable<uint32_t> newValue) override;
    void OnDischargingEnabledUntilChanged(DataModel::Nullable<uint32_t> newValue) override;
    void OnCircuitCapacityChanged(int64_t newValue) override;
    void OnMinimumChargeCurrentChanged(int64_t newValue) override;
    void OnMaximumChargeCurrentChanged(int64_t newValue) override;
    void OnMaximumDischargeCurrentChanged(int64_t newValue) override;
    void OnUserMaximumChargeCurrentChanged(int64_t newValue) override;
    void OnRandomizationDelayWindowChanged(uint32_t newValue) override;
    void OnNextChargeStartTimeChanged(DataModel::Nullable<uint32_t> newValue) override;
    void OnNextChargeTargetTimeChanged(DataModel::Nullable<uint32_t> newValue) override;
    void OnNextChargeRequiredEnergyChanged(DataModel::Nullable<int64_t> newValue) override;
    void OnNextChargeTargetSoCChanged(DataModel::Nullable<Percent> newValue) override;
    void OnApproximateEVEfficiencyChanged(DataModel::Nullable<uint16_t> newValue) override;
    void OnStateOfChargeChanged(DataModel::Nullable<Percent> newValue) override;
    void OnBatteryCapacityChanged(DataModel::Nullable<int64_t> newValue) override;
    void OnVehicleIDChanged(DataModel::Nullable<CharSpan> newValue) override;
    void OnSessionIDChanged(DataModel::Nullable<uint32_t> newValue) override;
    void OnSessionDurationChanged(DataModel::Nullable<uint32_t> newValue) override;
    void OnSessionEnergyChargedChanged(DataModel::Nullable<int64_t> newValue) override;
    void OnSessionEnergyDischargedChanged(DataModel::Nullable<int64_t> newValue) override;

private:
    static constexpr const char * LOG_TAG = "energy-evse";
};

} // namespace EnergyEvse
} // namespace Clusters
} // namespace app
} // namespace chip