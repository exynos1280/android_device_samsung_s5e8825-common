/*
 * Copyright (C) The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <libinit_variant.h>

#include "vendor_init.h"

static const variant_info_t a25xxx = {
    .device = "a25x",
    .model = "SM-A256B",
    .name = "a25xxx",
    .build_fingerprint = "samsung/a25xxx/essi:15/AP3A.240905.015.A2/A256BXXS8CYG4:user/release-keys",
    .build_desc = "a25xxx-user 15 AP3A.240905.015.A2 A256BXXS8CYG4 release-keys"
};

static const variant_info_t a25zhx = {
    .device = "a25x",
    .model = "SM-A2560",
    .name = "a25zhx",
    .build_fingerprint = "samsung/a25zhx/essi:15/AP3A.240905.015.A2/A2560ZHS7CYG4:user/release-keys",
    .build_desc = "a25zhx-user 15 AP3A.240905.015.A2 A2560ZHS7CYG4 release-keys"
};

static const variant_info_t a26xsub = {
    .device = "a26xs",
    .model = "SM-A266M",
    .name = "a26xsub",
    .build_fingerprint = "samsung/a26xsub/essi:15/AP3A.240905.015.A2/A266MUBS4AYH3:user/release-keys",
    .build_desc = "a26xsub-user 15 AP3A.240905.015.A2 A266MUBS4AYH3 release-keys"
};

static const variant_info_t a33xks = {
    .device = "a33x",
    .model = "SM-A336N",
    .name = "a33xks",
    .build_fingerprint = "samsung/a33xks/essi:15/AP3A.240905.015.A2/A336NKSSBFYH1:user/release-keys",
    .build_desc = "a33xks-user 15 AP3A.240905.015.A2 A336NKSSBFYH1 release-keys"
};

static const variant_info_t a33xnsdxx = {
    .device = "a33x",
    .model = "SM-A336E",
    .name = "a33xnsdxx",
    .build_fingerprint = "samsung/a33xnsdxx/essi:15/AP3A.240905.015.A2/A336EDXSEFYH2:user/release-keys",
    .build_desc = "a33xnsdxx-user 15 AP3A.240905.015.A2 A336EDXSEFYH2 release-keys"
};

static const variant_info_t a33xub = {
    .device = "a33x",
    .model = "SM-A336M",
    .name = "a33xub",
    .build_fingerprint = "samsung/a33xub/essi:15/AP3A.240905.015.A2/A336MUBSEFYH2:user/release-keys",
    .build_desc = "a33xub-user 15 AP3A.240905.015.A2 A336MUBSEFYH2 release-keys"
};

static const variant_info_t a33xzh = {
    .device = "a33x",
    .model = "SM-A3360",
    .name = "a33xzh",
    .build_fingerprint = "samsung/a33xzh/essi:15/AP3A.240905.015.A2/A3360ZHSEFYH2:user/release-keys",
    .build_desc = "a33xzh-user 15 AP3A.240905.015.A2 A3360ZHSEFYH2 release-keys"
};

static const variant_info_t a53xdcm = {
    .device = "a53x",
    .model = "SC-53C",
    .name = "a53xdcm",
    .build_fingerprint = "samsung/a53xdcm/essi:15/AP3A.240905.015.A2/SC53COMU1DYF2:user/release-keys",
    .build_desc = "a53xdcm-user 15 AP3A.240905.015.A2 SC53COMU1DYF2 release-keys"
};

static const variant_info_t a53xksx = {
    .device = "a53x",
    .model = "SM-A536N",
    .name = "a53xksx",
    .build_fingerprint = "samsung/a53xksx/essi:15/AP3A.240905.015.A2/A536NKSSCFYH1:user/release-keys",
    .build_desc = "a53xksx-user 15 AP3A.240905.015.A2 A536NKSSCFYH1 release-keys"
};

static const variant_info_t a53xnsxx = {
    .device = "a53x",
    .model = "SM-A536E",
    .name = "a53xnsxx",
    .build_fingerprint = "samsung/a53xnsxx/essi:15/AP3A.240905.015.A2/A536EXXSHFYI4:user/release-keys",
    .build_desc = "a53xnsxx-user 15 AP3A.240905.015.A2 A536EXXSHFYI4 release-keys"
};

static const variant_info_t a53xzc = {
    .device = "a53x",
    .model = "SM-A5360",
    .name = "a53xzc",
    .build_fingerprint = "samsung/a53xzc/essi:15/AP3A.240905.015.A2/A5360ZCSHFYH1:user/release-keys",
    .build_desc = "a53xzc-user 15 AP3A.240905.015.A2 A5360ZCSHFYH1 release-keys"
};

static const variant_info_t gta4xlsxx = {
    .device = "gta4xls",
    .model = "SM-P625",
    .name = "gta4xlsxx",
    .build_fingerprint = "samsung/gta4xlsxx/essi:15/AP3A.240905.015.A2/P625XXS6BYH1:user/release-keys",
    .build_desc = "gta4xlsxx-user 15 AP3A.240905.015.A2 P625XXS6BYH1 release-keys"
};

static const variant_info_t gta4xlswifixx = {
    .device = "gta4xlswifi",
    .model = "SM-P620",
    .name = "gta4xlswifixx",
    .build_fingerprint = "samsung/gta4xlswifixx/essi:15/AP3A.240905.015.A2/P620XXS7BYH1:user/release-keys",
    .build_desc = "gta4xlswifixx-user 15 AP3A.240905.015.A2 P620XXS7BYH1 release-keys"
};

static const variant_info_t m33xktt = {
    .device = "m33xktt",
    .model = "SM-M336K",
    .name = "m33xktt",
    .build_fingerprint = "samsung/m33xktt/essi:15/AP3A.240905.015.A2/M336KKSSBFH1:user/release-keys",
    .build_desc = "m33xktt-user 15 AP3A.240905.015.A2 M336KKSSBFH1 release-keys",
};

static const variant_info_t m34xdxx = {
    .device = "m34x",
    .model = "SM-M346B1",
    .name = "m34xdxx",
    .build_fingerprint = "samsung/m34xdxx/essi:15/AP3A.240905.015.A2/M346B1DXS8DYH1:user/release-keys",
    .build_desc = "m34xdxx-user 15 AP3A.240905.015.A2 M346B1DXS8DYH1 release-keys"
};

static const variant_info_t m34xins = {
    .device = "m34x",
    .model = "SM-E346B",
    .name = "m34xins",
    .build_fingerprint = "samsung/m34xins/essi:15/AP3A.240905.015.A2/E346BXXS8DYH1:user/release-keys",
    .build_desc = "m34xins-user 15 AP3A.240905.015.A2 E346BXXS8DYH1 release-keys"
};

static const variant_info_t m34xnsxx = {
    .device = "m34x",
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
    m33xktt,
    m34xdxx,
    m34xins,
    m34xnsxx,
};

void vendor_load_properties() {
    search_variant(variants);
}
