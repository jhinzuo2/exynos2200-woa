# Porting to r0s (Galaxy S22, Exynos 2200)

Status: **framework scaffold only. Nothing here has been built or booted.**
Base: the star2lte (S9+ / Exynos 9810) tree. The 9810 packages are untouched.

## What exists

| Path | Purpose |
|------|---------|
| `firmware/Silicon/Exynos2200Pkg/` | SoC package: `.dec` (GICv3 PCDs), `Exynos2200.h` register map, serial lib skeleton |
| `firmware/Platform/R0sPkg/` | Platform package: DSC/FDF, clean `PlatformMemoryMapLib`, `ArmPlatformLib`, reset lib, SimpleFramebufferDxe |

Convention: a base of `0` means "unknown". The memory-map lib skips zero-based windows.

## Deliberately NOT carried over from star2lte

- `PlatformBootManagerLib` (winload patching, USB debug, 9810 watchdog logic). DSC points at the path; write a minimal r0s version.
- UFS HC, HSI2C/GPIO/UART handoff DXEs, RAM manager, ACPI tables/platform DXE: all 9810 register-level code.
- `drivers/` (Exynos9810Ufs/Hsi2c, S6SY761Touch) and the 9810-specific installer flow.

## Known differences from 9810 that the scaffold already assumes

- GICv3 (distributor + redistributors), not GICv2.
- 8 cores in 3 clusters (X2 + 3x A710 + 4x A510): MPIDR layout and `ArmPlatformLib` core table still TODO.
- Display 1080x2340 expected (S9+ was 1440x2960); verify from the stock DT.
- Reset lib PMU offsets zeroed: the 9810 values are not valid.
- Timer frequency, CNTFRQ_EL0 behaviour: unknown, check on device.

## Bring-up order

1. Extract the stock r0s DT from a running device; fill `Exynos2200.h`.
2. UART base + serial lib, first UEFI banner (milestone 1).
3. GICv3 + timer, DRAM bank0, SEC/PEI to DXE.
4. Framebuffer reuse (sboot-lit), then boot-manager lib.
5. UFS, USB, ACPI, touch, then installer support.

## Open items before a build is possible

- DSC/FDF were mechanically renamed from star2lte; expect missing-file and PCD errors on first build.
- `ArmPlatformLib.c` core table needs the 2200 topology.
- Bootloader unlock/partition layout for SM-S901B must be verified separately; the installer must not target r0s until then.
