ifeq (eng,$(TARGET_BUILD_VARIANT))
VENDOR_LOG_LEVEL=I
else
VENDOR_LOG_LEVEL=S
endif

PRODUCT_PROPERTY_OVERRIDES += \
    persist.log.tag.artd=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.AnalyticsService=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.ArtService=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.b/223498680=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.EMS=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.CompatibilityChangeReporter=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.ContrastColorUtil=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.gralloc4=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.GRALLOC=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.hwcomposer=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.hwc-display=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.hwc-drm-crtc=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.hwc-drm-connector=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.hwc-drm-device=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.hwc-drm-display=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.hwc-drm-plane=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.HwcComposer=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.HWC2=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.HWUI=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.IntervalStats=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.KernelCpuUidActiveTimeReader=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.Nearby$(VENDOR_LOG_LEVEL)haring=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.QC2Buf=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.QC2V4l2Codec=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.SQLiteLog=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.tzts_daemon=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.TcUtils=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.Tracer=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.TrafficStats=$(VENDOR_LOG_LEVEL) \
    persist.log.tag.wificond=$(VENDOR_LOG_LEVEL)
