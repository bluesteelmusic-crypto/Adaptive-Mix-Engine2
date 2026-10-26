# Adaptive Mix Engine v0.9 — Windows VST3 build candidate

This package is **source code**, not a verified Windows VST3 binary. The GitHub Actions workflow compiles the plugin on a Windows runner and uploads a VST3 bundle if the build succeeds. It has not been run from this environment.

## Fastest Windows build (GitHub Actions)
1. Create a new private GitHub repository and upload the contents of this folder, including `.github/workflows/windows-vst3.yml`.
2. Open **Actions > Build Windows VST3 > Run workflow**.
3. If successful, download the `AdaptiveMixStereo-Windows-VST3` build artifact. Extract the artifact, preserving the `.vst3` bundle directory structure.
4. Copy the `.vst3` bundle to `C:\Program Files\Common Files\VST3\`. Restart Cubase 15 and check Studio > VST Plug-in Manager.
5. Insert on a stereo bus; test bypass, input/output gain, low control and strength; save and reopen a project.

## Local build (Visual Studio 2022)
Install CMake and JUCE 8, then in PowerShell:
```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DJUCE_DIR="C:/JUCE"
cmake --build build --config Release
```

## Scope and caveats
- This is a **stereo bus insert**, not the seven-stem reference-matching plugin. It cannot independently analyse or process seven tracks.
- The plugin currently uses a basic JUCE generic editor and does not load reference files.
- The 110 Hz band is a one-pole lowpass, not a precise 40–110 Hz bandpass.
- DSP-only C++ tests pass on Linux; actual JUCE/Windows compilation and Cubase recognition still need verification.
- If the Windows build fails, inspect the GitHub Actions logs and correct the source before treating the plugin as installable.
