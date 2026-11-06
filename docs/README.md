# CalypsoDAG
Welcome to the Minimal Viable Product (MVP) release of CalypsoDAG, a zero-dependency, low-level relational graph and hierarchical tree layout engine.

CalypsoDAG functions as a hidden, performant core infrastructure matrix layer designed for pure ISO C11 runtime execution. It is structured to act as an educational, "0-Ring ramping pathway"—omitting noisy autocomplete configurations by default to encourage developers to manually navigate and master low-level memory mechanics.


---
## 🛰 Core Philosophy
* Zero Runtime Heap Overhead: Memory manipulation is achieved entirely through upfront static buffer layouts and calling-context memory blocks. You will find zero calls to malloc, calloc, or free within the graph core.
* Insulated Processing: Relational pointer math transforms cleanly down the wire, completely isolated from <stdio.h> side effects to maximize performance metrics and determinism.
* Separation of Concerns: The core operational library is completely stateless, passing telemetry events out to a bounded static character stream registry inside an isolated logging module.


---
## 🧬 Type Agnosticism via Intrusive Composition
To maintain zero-overhead data mapping without void pointers or generic metadata arrays, CalypsoDAG utilizes intrusive structures. Custom payloads, layout shapes, or asset parameters are bound to the engine by embedding CKgraphNodeHeader as the absolute first member of your struct layout.

Because of this binary alignment, a pointer to your custom type is perfectly equivalent to a generic node handle:
```c
// How a developer attaches custom asset data to a Calypso node:
typedef struct {
    CKgraphNodeHeader header; // CRITICAL: Must be the absolute first member
    const char* face_label;
    uint32_t cell_color;
} TelestoFaceMesh;

// Instantiation maps directly into the engine functions with zero conversion sugar:
TelestoFaceMesh parent_face = { .face_label = "Compound_Root_01" };
TelestoFaceMesh child_face  = { .face_label = "Rectified_Leaf_A1", .cell_color = 0xFF0000 };

CKdag_AppendChild((CKgraphNodeHandle)&parent_face, (CKgraphNodeHandle)&child_face);
```


---
## 🕷 Chronological Code Study: The Pre-Order Infinite Loop Vector
This codebase is intentionally packaged and frozen with a known structural loop vector inside src/CalypsoDAG_core.c. As part of an interactive systems engineering workflow, the CKgraph_TraversePreOrder function is left with its initial, unhardened iterative backtracking algorithm intact.

### Main Branch Health Guarantee
The structural layout, memory boundaries, and detachment pipelines provided inside the code are verified. Any RevisionTest running out of the main branch is fully guaranteed to compile natively and operate successfully to a safe exit 0 status under standard symmetric tree distributions. The loop anomaly is restricted strictly to asymmetric layout bounds or custom horizontal cyclic injections.

### The Behavior
When evaluating an asymmetric hierarchy or layout graphs with horizontal looping nodes, the upward backtracking engine:
```c
while (parentNode && parentNode != root) {
    if (parentNode->nextSibling) {
        current = parentNode->nextSibling;
        break; 
    }
    parentNode = parentNode->parent;
}
```
lacks an explicit state register or historical prev pointer to track which deep subdivision levels have already been exited. The engine will consistently re-enter completed sub-branches, resulting in an execution loop stall.

### How to Use It
1. Use RevisionTest01.c to see how the system operates under standard, symmetric planar tree distributions.
2. Intentionally create an asymmetric graph structure or introduce horizontal cyclic relations inside a custom unit test.
3. Set your runtime logging layer to CK_LOG_ALL using CKdag_setLogLevel(CK_LOG_ALL).
4. Run the code, observe the execution stall, and call CKdag_flushLog(CK_LOG_CONSOLE) via your terminal to trace exactly how the tracking context fails to exit the loops.

This loop behavior will be refactored and proven safe in future revisions using the data harvested by this version's telemetry buffer!


---
## 🛠 Project Directory Layout
The implementation is decoupled into distinct public interfaces, forward declarations, structural type metrics, and localized utility translations under the ecosystem's foundational tier guidelines:
```text
CalypsoDAG/
├─ CMakeLists.txt              # Automated build orchestration contract
├─ docs/...                    # Project .md document collection
├─ src/                        #
│   ├── CalypsoDAG_core.c      # Stateless Structural Relational Logic
│   └── CalypsoDAG_log.c       # State-Managed Bounded Logging Subsystem
├─ include/                    #
│   ├── CalypsoDAG_typedef.h   # Concrete Data Layout Struct Definitions
│   ├── CalypsoDAG_typefwd.h   # Opaque Struct Declarations & Configuration Enums
│   └── CalypsoDAG.h           # Public Function Contract API
└─ unittest/                   # 
    └── RevisionTestXX.c       # One of our unit tests / applications
```


---
## Windows Powershell Quickstart: Building via Clang + Ninja
CalypsoDAG is optimized for modern **LLVM/Clang-LLD toolchains**. This quickstart guide uses **MSYS2** on Windows to automate compilation.

### 0. Install MSYS2 and CMAKE
Include CMake in your enviroment PATH during installation - It's an option during installion - Don't miss it!
> MSYS2 does **not** need to be in your path if you follow our example and point strait to the executables.

### 1. Prerequisites (Via MSYS2 Clang64 Shell)
Ensure you have the required compilers and build generators installed via `pacman`:

```bash
pacman -S mingw-w64-clang-x86_64-clang mingw-w64-clang-x86_64-ninja
```

### 2. Compilation
Download the repository, open your PowerShell, and execute our automatic compilation steps:

```powershell
# Navigate to the CharikloOS directory you just obtained
# You may have to unpack a compressed directory
cd path/to/CalypsoDAG

# Create an isolated build directory
# ALWAYS start with an empty build directory
mkdir build
cd build

# Configure the pure C workspace using the explicit compiler paths
cmake .. -G "Ninja" -DCMAKE_C_COMPILER="C:/msys64/clang64/bin/clang.exe" -DCMAKE_MAKE_PROGRAM="C:/msys64/clang64/bin/ninja.exe"

# Execute the compilation build target
cmake --build .
```

Upon a successful build, the automated system outputs an optimized binary straight to your `build/bin/` folder. Run it to launch whichever RevisionTest or build is active!

CRITICAL DEVELOPMENT NOTE: Never build or execute unverified, raw structural code scripts on your host environment without performing an upfront manual layout audit.
> For users seeking an entirely sandboxed experience, a comprehensive, unified "WDAG build environment" is currently a Work-In-Progress (WIP) within the main Tethys repository. Once live, it will provide fully automated fallback scripts, pre-baked configurations, download-once portable binaries, and an isolated sandbox for the entire stack.