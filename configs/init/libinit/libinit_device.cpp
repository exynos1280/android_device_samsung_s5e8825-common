/*
 * Copyright (C) The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <libinit_device.h>
#include <libinit_utils.h>

using android::base::GetProperty;

#define CODENAME_PROP "ro.product.vendor.device"

void search_device(const std::vector<device_info_t> devices) {
    std::string codename_prop = GetProperty(CODENAME_PROP, "");

    for (const auto& device : devices) {
        if ((device.codename == "" || device.codename == codename_prop)) {
            set_device_props(device);
            break;
        }
    }
}

void set_device_props(const device_info_t device) {
    property_override("bluetooth.device.default_name", device.full_name);
}
