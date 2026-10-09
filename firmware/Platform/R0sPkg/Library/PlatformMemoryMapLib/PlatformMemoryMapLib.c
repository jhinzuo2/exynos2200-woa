/** @file
  PlatformMemoryMapLib - Exynos 2200 / Galaxy S22 (r0s).

  Builds the ARM memory region descriptors used for the UEFI page tables.
  Every base/size comes from Silicon/Exynos2200Pkg/Include/Platform/Exynos2200.h.

  Rules:
   - DRAM: WRITE_BACK (cached). MMIO: DEVICE (non-cached).
   - TrustZone / modem / GPU-firmware carveouts must NOT be exposed as RAM.
   - Windows with Base == 0 are unverified placeholders and are SKIPPED, so a
     TODO value never maps physical address 0.

  BRING-UP ORDER: map only the UART first, then GIC, then DRAM bank0, and add
  further windows one at a time as each is verified from the r0s device tree.
**/

#include <Library/ArmPlatformLib.h>
#include <Library/ArmLib.h>
#include <Library/DebugLib.h>
#include <Platform/Exynos2200.h>

#define MMAP_DRAM  ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define MMAP_DEV   ARM_MEMORY_REGION_ATTRIBUTE_DEVICE

typedef struct {
  UINT64         Base;
  UINT64         Size;
  CONST CHAR8    *Name;
} MMIO_REGION;

STATIC CONST MMIO_REGION  mMmioRegions[] = {
  { EXYNOS_UART_BASE,   EXYNOS_UART_SIZE,   "UART"   },
  { EXYNOS_GICD_BASE,   EXYNOS_GICD_SIZE,   "GICD"   },
  { EXYNOS_GICR_BASE,   EXYNOS_GICR_SIZE,   "GICR"   },
  { EXYNOS_CHIPID_BASE, EXYNOS_CHIPID_SIZE, "CHIPID" },
  { EXYNOS_PMU_BASE,    EXYNOS_PMU_SIZE,    "PMU"    },
  { EXYNOS_UFS_HCI_BASE, EXYNOS_UFS_HCI_SIZE, "UFS_HCI" },
  { EXYNOS_USBDRD_BASE, EXYNOS_USBDRD_SIZE, "USBDRD" },
};

#define MMIO_REGION_COUNT  (sizeof (mMmioRegions) / sizeof (mMmioRegions[0]))

VOID
ArmPlatformGetVirtualMemoryMap (
  OUT ARM_MEMORY_REGION_DESCRIPTOR  **MemoryTable
  )
{
  // DRAM + framebuffer + MMIO windows + terminator.
  STATIC ARM_MEMORY_REGION_DESCRIPTOR  Table[MMIO_REGION_COUNT + 3];
  UINTN                                Index;
  UINTN                                Region;

  ASSERT (MemoryTable != NULL);
  Index = 0;

  if (EXYNOS_DRAM_SIZE != 0) {
    Table[Index].PhysicalBase = EXYNOS_DRAM_BASE;
    Table[Index].VirtualBase  = EXYNOS_DRAM_BASE;
    Table[Index].Length       = EXYNOS_DRAM_SIZE;
    Table[Index].Attributes   = MMAP_DRAM;
    Index++;
  }

  if (EXYNOS_FB_BASE != 0) {
    Table[Index].PhysicalBase = EXYNOS_FB_BASE;
    Table[Index].VirtualBase  = EXYNOS_FB_BASE;
    Table[Index].Length       = EXYNOS_FB_SIZE;
    Table[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;
    Index++;
  }

  for (Region = 0; Region < MMIO_REGION_COUNT; Region++) {
    if ((mMmioRegions[Region].Base == 0) || (mMmioRegions[Region].Size == 0)) {
      DEBUG ((DEBUG_INFO, "R0s: skip unverified MMIO window %a\n", mMmioRegions[Region].Name));
      continue;
    }

    Table[Index].PhysicalBase = mMmioRegions[Region].Base;
    Table[Index].VirtualBase  = mMmioRegions[Region].Base;
    Table[Index].Length       = mMmioRegions[Region].Size;
    Table[Index].Attributes   = MMAP_DEV;
    Index++;
  }

  // Terminator.
  Table[Index].PhysicalBase = 0;
  Table[Index].VirtualBase  = 0;
  Table[Index].Length       = 0;
  Table[Index].Attributes   = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  *MemoryTable = Table;
}
