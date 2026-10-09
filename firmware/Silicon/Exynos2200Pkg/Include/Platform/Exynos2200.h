/** @file
  Exynos2200.h - central SoC register-block map for the Galaxy S22 family,
  Exynos 2200 (r0s = SM-S901B).

  Single source of truth for MMIO bases/sizes and framebuffer geometry, used by
  PlatformMemoryMapLib and the platform drivers (same role as Exynos9810.h).

  Convention (inherited from the S9+ port): a base of 0 means "unknown / not
  configured". The memory map lib skips zero-based windows so a placeholder
  never maps PA 0.

  NOTHING here is verified yet. Every value is TODO-VERIFY and must be filled
  from the r0s device tree (decompile the stock DT on the device) before first
  boot. Do not copy 9810 addresses: the SoC layout is entirely different.
**/

#ifndef EXYNOS2200_H_
#define EXYNOS2200_H_

// DRAM: base is the usual Exynos 0x80000000; banks/size TODO-VERIFY.
#define EXYNOS_DRAM_BASE          0x0000000080000000ULL   // TODO-VERIFY
#define EXYNOS_DRAM_SIZE          0x0000000000000000ULL   // TODO-VERIFY

// Debug UART (Samsung UART/USI, not 16550/PL011).
#define EXYNOS_UART_BASE          0x0000000000000000ULL   // TODO-VERIFY
#define EXYNOS_UART_SIZE          0x0000000000001000ULL

// GICv3: distributor + redistributor region (no GICC/GICH/GICV window needed).
#define EXYNOS_GICD_BASE          0x0000000000000000ULL   // TODO-VERIFY
#define EXYNOS_GICD_SIZE          0x0000000000010000ULL
#define EXYNOS_GICR_BASE          0x0000000000000000ULL   // TODO-VERIFY
#define EXYNOS_GICR_SIZE          0x0000000000000000ULL   // TODO-VERIFY (8 cores x 0x20000)

// Architected timer: CNTFRQ_EL0 may be unprogrammed by the stock bootloader (as on 9810).
// Frequency TODO-VERIFY on device before hardcoding.
#define EXYNOS_ARCH_TIMER_FREQ_HZ 0u                      // TODO-VERIFY

// PMU / chip ID
#define EXYNOS_PMU_BASE           0x0000000000000000ULL   // TODO-VERIFY
#define EXYNOS_PMU_SIZE           0x0000000000010000ULL
#define EXYNOS_CHIPID_BASE        0x0000000000000000ULL   // TODO-VERIFY
#define EXYNOS_CHIPID_SIZE        0x0000000000000100ULL

// UFS (UFS 3.1 host controller): TODO-VERIFY
#define EXYNOS_UFS_HCI_BASE       0x0000000000000000ULL   // TODO-VERIFY
#define EXYNOS_UFS_HCI_SIZE       0x0000000000002000ULL

// USB DRD (xHCI-compatible)
#define EXYNOS_USBDRD_BASE        0x0000000000000000ULL   // TODO-VERIFY
#define EXYNOS_USBDRD_SIZE        0x0000000000010000ULL

// Display (sboot-lit framebuffer is reused first). Panel geometry for r0s
// (FHD+ 2340x1080) is the expected value, TODO-VERIFY from the stock DT.
#define EXYNOS_FB_BASE            0x0000000000000000ULL   // TODO-VERIFY (reserved-memory carveout)
#define EXYNOS_FB_WIDTH           1080                     // TODO-VERIFY
#define EXYNOS_FB_HEIGHT          2340                     // TODO-VERIFY
#define EXYNOS_FB_BYTES_PER_PIXEL 4
#define EXYNOS_FB_STRIDE_PIXELS   EXYNOS_FB_WIDTH
#define EXYNOS_FB_SIZE            ((UINT64)EXYNOS_FB_STRIDE_PIXELS * \
                                   EXYNOS_FB_HEIGHT * EXYNOS_FB_BYTES_PER_PIXEL)

#endif // EXYNOS2200_H_
