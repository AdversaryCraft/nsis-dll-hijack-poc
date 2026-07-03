# nsis-dll-hijack-poc

## Overview

Software installers are often treated as trusted executables, yet they perform numerous runtime operations before the target application is installed.

This repository explores the **Nullsoft Scriptable Install System (NSIS)** plugin architecture by examining how plugin DLLs are extracted into temporary directories, loaded into memory, and resolved through exported functions.

Rather than focusing on exploitation, this project demonstrates how reverse engineering installer runtimes can improve:

- Detection engineering
- Purple team validation
- Windows internals research
- Runtime instrumentation

---

## Research Goals

This project investigates:

- NSIS runtime architecture
- Plugin extraction workflow
- Temporary directory creation
- Plugin export resolution
- Runtime module loading
- Export forwarding
- Detection opportunities for defenders

---

## Repository Structure

```text
.
├── ccsetup564.exe             # Legitimate NSIS installer (not included)
├── launch.wsf                 # Lab launcher
├── nsDialogs.dll              # Instrumented proxy DLL
├── msvcrt_original.dll        # Renamed original plugin
├── screenshots/
├── diagrams/
└── README.md
```

---

## Runtime Workflow

```text
User launches installer
        │
        ▼
User approves UAC
        │
        ▼
NSIS creates nseXXXX.tmp
        │
        ▼
Plugin DLLs extracted
        │
        ▼
Windows loads nsDialogs.dll
        │
        ▼
Proxy validates runtime
        │
        ▼
Original exports continue
        │
        ▼
Installation completes
```

---

## Reverse Engineering Process

The research consists of several stages.

### 1. Installer Identification

The installer is identified using Detect It Easy (DIE) to confirm it is an NSIS package.

---

### 2. Runtime Extraction

During execution, NSIS creates a randomized directory similar to:

```text
%LOCALAPPDATA%\Temp\nseXXXX.tmp
```

The installer extracts multiple plugin DLLs including:

- nsDialogs.dll
- System.dll
- UserInfo.dll

---

### 3. Module Loading

Process Monitor is used to observe:

```
Load Image
```

events generated when the installer loads extracted plugins.

---

### 4. Export Analysis

The exported interface of **nsDialogs.dll** is inspected using x64dbg.

Important exports include:

- Create
- CreateControl
- Show
- OnClick
- OnNotify
- SelectFolderDialog

---

### 5. Export Forwarding

A compatible proxy DLL is used to validate runtime behavior while forwarding remaining exports to the original plugin.

This demonstrates the plugin contract expected by the installer without disrupting normal execution.

---

## Detection Opportunities

Potential telemetry includes:

- DLL creation inside `nse*.tmp`
- Image Load events
- Module signature validation
- Unsigned module loads
- Runtime DLL replacement
- Temporary directory monitoring

---

## Purple Team Validation

Suggested validation objectives:

- Can the EDR detect DLL loads from `nse*.tmp`?
- Are unsigned modules identified?
- Are Image Load events collected?
- Are temporary installer directories monitored?
- Is module signing verified?

---

## Screenshots

The accompanying article includes:

- Detect It Easy
- Process Monitor
- Temporary extraction directory
- Export table
- x64dbg analysis
- Runtime validation
- Successful installation

---

## Blog

A complete technical write-up is available on **AdversaryCraft**:

> **Inside the NSIS Runtime: Plugin Architecture, Temporary Extraction, and Export Forwarding**

---

## References

- NSIS Documentation
- Microsoft Windows Loader Documentation
- Process Monitor
- Detect It Easy
- x64dbg

---

## Disclaimer

This repository is provided solely for educational purposes, reverse engineering research, and defensive security validation within authorized laboratory environments.

The code and documentation are intended to help security professionals better understand Windows installer internals and improve defensive telemetry. They are **not** intended for unauthorized use against systems you do not own or have permission to test.
