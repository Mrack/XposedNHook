/*
 * This file is part of QBDI.
 *
 * Copyright 2017 - 2023 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef QBDI_CONFIG_H_
#define QBDI_CONFIG_H_

/* #undef QBDI_ARCH_ARM */
#define QBDI_ARCH_AARCH64 1
/* #undef QBDI_ARCH_X86 */
/* #undef QBDI_ARCH_X86_64 */

/* #undef QBDI_PLATFORM_WINDOWS */
/* #undef QBDI_PLATFORM_LINUX */
#define QBDI_PLATFORM_ANDROID 1
/* #undef QBDI_PLATFORM_OSX */
/* #undef QBDI_PLATFORM_IOS */

/* #undef QBDI_NOT_AVX_SUPPORT */

/* #undef QBDI_BITS_32 */
#define QBDI_BITS_64  1

/* #undef QBDI_LOG_DEBUG */

/* #undef QBDI_EXPORT_SYM */

#ifdef __cplusplus
namespace QBDI {

static constexpr bool is_android = 1;
static constexpr bool is_linux   = 0;
static constexpr bool is_osx     = 0;
static constexpr bool is_ios     = 0;
static constexpr bool is_windows = 0;


static constexpr bool is_arm     = 0;
static constexpr bool is_aarch64 = 1;
static constexpr bool is_x86     = 0;
static constexpr bool is_x86_64  = 0;

static constexpr bool it_bits_32 = 0;
static constexpr bool is_bits_64 = 1;

static constexpr bool has_debug_log = 0;
}
#endif // __cplusplus

#endif // QBDI_CONFIG_H_
