/*
 * Copyright (C) The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "vendor_init.h"
#include <libinit_dalvik_heap.h>
#include <libinit_device.h>
#include <libinit_utils.h>
#include <libinit_variant.h>

#include <android-base/properties.h>

#define NFC_CHIP_NXP "NXP"

using android::base::GetProperty;

/*
 * Devices
 */
static const device_info_t a25x = {
  .codename = "a25x",
  .full_name = "Galaxy A25 5G",
  .nfc_chip = "ST",
  .adaptive_rr = true,
  .side_fp = true,
};

static const device_info_t a26xs = {
  .codename = "a26xs",
  .full_name = "Galaxy A26 5G",
  .nfc_chip = "ST",
  .adaptive_rr = true,
  .side_fp = true,
};

static const device_info_t a33x = {
  .codename = "a33x",
  .full_name = "Galaxy A33 5G",
  .nfc_chip = "SLSI",
  .adaptive_rr = true,
  .udfps = true,
};

static const device_info_t a53x = {
  .codename = "a53x",
  .full_name = "Galaxy A53 5G",
  .nfc_chip = "SLSI",
  .adaptive_rr = true,
  .udfps = true,
};

static const device_info_t f34x = {
  .codename = "f34x",
  .full_name = "Galaxy F34 5G",
  .nfc_chip = "ST",
  .adaptive_rr = true,
  .side_fp = true,
};

static const device_info_t gta4xls = {
  .codename = "gta4xls",
  .full_name = "Galaxy Tab S6 Lite",
};

static const device_info_t gta4xlswifi = {
  .codename = "gta4xlswifi",
  .full_name = "Galaxy Tab S6 Lite (WiFi)",
};

static const device_info_t m33x = {
  .codename = "m33x",
  .full_name = "Galaxy M33 5G",
  .nfc_chip = "NXP",
  .adaptive_rr = true,
  .side_fp = true,
};

static const device_info_t m34x = {
  .codename = "m34x",
  .full_name = "Galaxy M34 5G",
  .nfc_chip = "ST",
  .adaptive_rr = true,
  .side_fp = true,
};

static const std::vector<device_info_t> devices = {
  a25x,
  a26xs,
  a33x,
  a53x,
  f34x,
  gta4xls,
  gta4xlswifi,
  m33x,
  m34x,
};

/*
 * Variants
 */
static const variant_info_t a25xxx = {
  .model = "SM-A256B",
  .name = "a25xxx",
  .build_fingerprint = "samsung/a25xxx/essi:15/AP3A.240905.015.A2/A256BXXS8CYG4:user/release-keys",
  .build_desc = "a25xxx-user 15 AP3A.240905.015.A2 A256BXXS8CYG4 release-keys"
};

static const variant_info_t a25zhx = {
  .model = "SM-A2560",
  .name = "a25zhx",
  .build_fingerprint = "samsung/a25zhx/essi:15/AP3A.240905.015.A2/A2560ZHS7CYG4:user/release-keys",
  .build_desc = "a25zhx-user 15 AP3A.240905.015.A2 A2560ZHS7CYG4 release-keys"
};

static const variant_info_t a26xsub = {
  .model = "SM-A266M",
  .name = "a26xsub",
  .build_fingerprint = "samsung/a26xsub/essi:15/AP3A.240905.015.A2/A266MUBS4AYH3:user/release-keys",
  .build_desc = "a26xsub-user 15 AP3A.240905.015.A2 A266MUBS4AYH3 release-keys"
};

static const variant_info_t a33xks = {
  .model = "SM-A336N",
  .name = "a33xks",
  .build_fingerprint = "samsung/a33xks/essi:15/AP3A.240905.015.A2/A336NKSSBFYH1:user/release-keys",
  .build_desc = "a33xks-user 15 AP3A.240905.015.A2 A336NKSSBFYH1 release-keys"
};

static const variant_info_t a33xnsdxx = {
  .model = "SM-A336E",
  .name = "a33xnsdxx",
  .build_fingerprint = "samsung/a33xnsdxx/essi:15/AP3A.240905.015.A2/A336EDXSEFYH2:user/release-keys",
  .build_desc = "a33xnsdxx-user 15 AP3A.240905.015.A2 A336EDXSEFYH2 release-keys"
};

static const variant_info_t a33xub = {
  .model = "SM-A336M",
  .name = "a33xub",
  .build_fingerprint = "samsung/a33xub/essi:15/AP3A.240905.015.A2/A336MUBSEFYH2:user/release-keys",
  .build_desc = "a33xub-user 15 AP3A.240905.015.A2 A336MUBSEFYH2 release-keys"
};

static const variant_info_t a33xzh = {
  .model = "SM-A3360",
  .name = "a33xzh",
  .build_fingerprint = "samsung/a33xzh/essi:15/AP3A.240905.015.A2/A3360ZHSEFYH2:user/release-keys",
  .build_desc = "a33xzh-user 15 AP3A.240905.015.A2 A3360ZHSEFYH2 release-keys"
};

static const variant_info_t a53xdcm = {
  .model = "SC-53C",
  .name = "a53xdcm",
  .build_fingerprint = "samsung/a53xdcm/essi:15/AP3A.240905.015.A2/SC53COMU1DYF2:user/release-keys",
  .build_desc = "a53xdcm-user 15 AP3A.240905.015.A2 SC53COMU1DYF2 release-keys",
  .nfc_chip = "NXP",
};

static const variant_info_t a53xksx = {
  .model = "SM-A536N",
  .name = "a53xksx",
  .build_fingerprint = "samsung/a53xksx/essi:15/AP3A.240905.015.A2/A536NKSSCFYH1:user/release-keys",
  .build_desc = "a53xksx-user 15 AP3A.240905.015.A2 A536NKSSCFYH1 release-keys"
};

static const variant_info_t a53xnsxx = {
  .model = "SM-A536E",
  .name = "a53xnsxx",
  .build_fingerprint = "samsung/a53xnsxx/essi:15/AP3A.240905.015.A2/A536EXXSHFYI4:user/release-keys",
  .build_desc = "a53xnsxx-user 15 AP3A.240905.015.A2 A536EXXSHFYI4 release-keys"
};

static const variant_info_t a53xzc = {
  .model = "SM-A5360",
  .name = "a53xzc",
  .build_fingerprint = "samsung/a53xzc/essi:15/AP3A.240905.015.A2/A5360ZHSHFYI1:user/release-keys",
  .build_desc = "a53xzc-user 15 AP3A.240905.015.A2 A5360ZHSHFYI1 release-keys"
};

static const variant_info_t gta4xlsxx = {
  .model = "SM-P625",
  .name = "gta4xlsxx",
  .build_fingerprint = "samsung/gta4xlsxx/essi:15/AP3A.240905.015.A2/P625XXS6BYH1:user/release-keys",
  .build_desc = "gta4xlsxx-user 15 AP3A.240905.015.A2 P625XXS6BYH1 release-keys"
};

static const variant_info_t gta4xlswifixx = {
  .model = "SM-P620",
  .name = "gta4xlswifixx",
  .build_fingerprint = "samsung/gta4xlswifixx/essi:15/AP3A.240905.015.A2/P620XXS7BYH1:user/release-keys",
  .build_desc = "gta4xlswifixx-user 15 AP3A.240905.015.A2 P620XXS7BYH1 release-keys"
};

static const variant_info_t m33xins = {
  .model = "SM-M336BU",
  .name = "m33xins",
  .build_fingerprint = "samsung/m33xins/essi:15/AP3A.240905.015.A2/M336BUXXSDFYH1:user/release-keys",
  .build_desc = "m33xins-user 15 AP3A.240905.015.A2 M336BUXXSDFYH1 release-keys",
};

static const variant_info_t m33xktt = {
  .model = "SM-M336K",
  .name = "m33xktt",
  .build_fingerprint = "samsung/m33xktt/essi:15/AP3A.240905.015.A2/M336KKSSBFH1:user/release-keys",
  .build_desc = "m33xktt-user 15 AP3A.240905.015.A2 M336KKSSBFH1 release-keys",
};

static const variant_info_t m34xdxx = {
  .model = "SM-M346B1",
  .name = "m34xdxx",
  .build_fingerprint = "samsung/m34xdxx/essi:15/AP3A.240905.015.A2/M346B1DXS8DYH1:user/release-keys",
  .build_desc = "m34xdxx-user 15 AP3A.240905.015.A2 M346B1DXS8DYH1 release-keys"
};

static const variant_info_t m34xins = {
  .model = "SM-E346B",
  .name = "m34xins",
  .build_fingerprint = "samsung/m34xins/essi:15/AP3A.240905.015.A2/E346BXXS8DYH1:user/release-keys",
  .build_desc = "m34xins-user 15 AP3A.240905.015.A2 E346BXXS8DYH1 release-keys"
};

static const variant_info_t m34xnsxx = {
  .model = "SM-M346B",
  .name = "m34xnsxx",
  .build_fingerprint = "samsung/m34xnsxx/essi:15/AP3A.240905.015.A2/M346BXXS8DYH1:user/release-keys",
  .build_desc = "m34xnsxx-user 15 AP3A.240905.015.A2 M346BXXS8DYH1 release-keys"
};

static const std::vector<variant_info_t> variants = {
  a25xxx,
  a25zhx,
  a26xsub,
  a33xks,
  a33xnsdxx,
  a33xub,
  a33xzh,
  a53xdcm,
  a53xksx,
  a53xnsxx,
  a53xzc,
  gta4xlsxx,
  gta4xlswifixx,
  m33xins,
  m33xktt,
  m34xdxx,
  m34xins,
  m34xnsxx,
};

void vendor_load_properties() {
  search_device(devices);
  search_variant(variants);

  std::string model = GetProperty("ro.boot.em.model", "");
  set_ro_boot_prop("product.hardware.sku", model);
  set_ro_build_prop("model", model, true);
  set_ro_build_prop("product", model, false);

  std::string nfc_chip = GetProperty("ro.vendor.nfc.feature.chipname", "");
  if (nfc_chip == NFC_CHIP_NXP) {
    property_override("ro.camera.notify_nfc", "1");
    property_override("ro.vendor.nfc.feature.chipname", "NXP_SN100U");
    property_override("ro.vendor.nfc.info.antpos", "16");
    property_override("ro.vendor.nfc.info.antposX", "25.4");
    property_override("ro.vendor.nfc.info.antposY", "35");
    property_override("ro.vendor.nfc.info.deviceFoldable", "false");
    property_override("ro.vendor.nfc.info.deviceHeight", "165.4");
    property_override("ro.vendor.nfc.info.deviceWidth", "76.9");
    property_override("ro.vendor.nfc.support.advancedsetting", "false");
    property_override("ro.vendor.nfc.support.autoselect", "true");
    property_override("ro.vendor.nfc.support.defaultaid", "true");
    property_override("ro.vendor.nfc.support.ese", "false");
    property_override("ro.vendor.nfc.support.nonaid", "true");
    property_override("ro.vendor.nfc.support.othercategory", "true");
    property_override("ro.vendor.nfc.support.uicc", "true");
  }

  set_dalvik_heap();
}
