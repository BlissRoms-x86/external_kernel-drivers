#
# Copyright (C) 2018 The Android-x86 Open Source Project
#
# Licensed under the GNU General Public License Version 2 or later.
# You may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.gnu.org/licenses/gpl.html
#

LOCAL_PATH := $(my-dir)
LOCAL_MODULE := $(notdir $(LOCAL_PATH))
EXTRA_KERNEL_MODULE_PATH_$(LOCAL_MODULE) := $(LOCAL_PATH)

# parts of build/tasks/kernel.mk

TARGET_KERNEL_ARCH ?= $(TARGET_ARCH)
WL_PATH := $(TARGET_OUT_INTERMEDIATES)/kmodule/wl
WL_SRC := $(WL_PATH)/hybrid-v35$(if $(filter x86,$(TARGET_KERNEL_ARCH)),,_64)-nodebug-pcoem-6_30_223_271.tar.gz
WL_LIB := $(WL_PATH)/lib$(if $(filter x86,$(TARGET_KERNEL_ARCH)),32,64)

WL_PATCHES := \
	wl.patch \
	linux-recent.patch \
	linux-48.patch \
	linux-411.patch \
	linux-412.patch \
	linux-415.patch \

$(WL_SRC):
	@echo Downloading $(@F)...
	$(hide) mkdir -p $(@D) && curl -k https://docs.broadcom.com/docs-and-downloads/docs/linux_sta/$(@F) > $@

$(WL_LIB): $(WL_SRC) $(addprefix $(LOCAL_PATH)/,$(WL_PATCHES))
	$(hide) tar zxf $< -C $(@D) --overwrite -m && \
		rm -rf $@ && mv $(@D)/lib $@ && touch $@ && \
		cat $(filter %.patch,$^) | patch -p1 -d $(@D)

$(WL_PATH): $(WL_LIB)
