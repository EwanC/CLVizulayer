# Leela Chess Zero

[Leela Chess Zero](https://lczero.org/) or "lc0" is an [open source](https://github.com/LeelaChessZero/lc0)
AI chess engine that supports multiple backends, one of which is OpenCL.

CLVizulayer to produce a DOT graph for the OpenCL backend by using the
[T78 network](http://training.lczero.org/get_network?sha=d0ed346c32fbcc9eb2f0bc7e957d188c8ae428ee3ef7291fd5aa045fc6ef4ded)
from the [Best Nets for Lc0 documentation](https://lczero.org/dev/wiki/best-nets-for-lc0). This was
run on single board position for 1 node to produce a linear graph of device tasks.

```
$ OPENCL_LAYERS=libCLVizuLayer.so VIZ_COLOR=1 VIZ_VERBOSE=1 ./lc0 benchmark -w weights/T78 --num-positions=1 --nodes=1
       _
|   _ | |
|_ |_ |_| v0.31.2 built Aug  8 2025
Loading weights file from: weights/T78
Creating backend [opencl]...
OpenCL, maximum batch size set to 16.
Initializing OpenCL.
Detected 1 OpenCL platforms.
Platform version: OpenCL 3.0 PoCL 7.1-pre main-0-g92acb3324  Linux, Debug+Asserts, RELOC, LLVM 19.1.1, SLEEF, POCL_DEBUG
Platform profile: FULL_PROFILE
Platform name:    Portable Computing Language
Platform vendor:  The pocl project
Device ID:      0
Device name:    cpu-znver4-AMD Ryzen 7 PRO 8840HS w/ Radeon 780M Graphics
Device type:    CPU
Device vendor:  AuthenticAMD
Device driver:  7.1-pre main-0-g92acb3324
Device speed:   5134 MHZ
Device cores:   16 CU
Device score:   1030
Selected platform: Portable Computing Language
Selected device: cpu-znver4-AMD Ryzen 7 PRO 8840HS w/ Radeon 780M Graphics
with OpenCL 3.0 capability.
Loaded existing SGEMM tuning for batch size 16.
Wavefront/Warp size: 8

Max workgroup size: 4096
Max workgroup dimensions: 4096 4096 4096

Position: 1/1 rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
Benchmark time 150 ms, 1 nodes, -1 nps, move e2e4
bestmove e2e4

===========================
Total time (ms) : 153
Nodes searched  : 1
Nodes/second    : 6
```

This creates the `trace.dot` trace, which can be rendered in unicode boxart with
`graph-easy trace.dot -as boxart` as:

```
┌−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−┐
╎              clFinish()              ╎
╎                                      ╎
╎ ┌──────────────────────────────────┐ ╎
╎ │       clEnqueueWriteBuffer       │ ╎
╎ │         blocking = False         │ ╎
╎ │     buffer = 0x71c044007b40      │ ╎
╎ │            offset = 0            │ ╎
╎ │           size = 28672           │ ╎
╎ │       ptr = 0x71c045052080       │ ╎ ─┐
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │ blocking host wait
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006810    │ ╎  │
╎ │    name = global_avg_pooling     │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006990    │ ╎  │
╎ │         name = apply_se          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005f30    │ ╎  │
╎ │       name = in_transform        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006540    │ ╎  │
╎ │ name = out_transform_fused_bn_in │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006110    │ ╎  │
╎ │       name = XgemmBatched        │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006320    │ ╎  │
╎ │  name = out_transform_fused_bn   │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006b70    │ ╎  │
╎ │         name = policymap         │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044003bf0    │ ╎  │
╎ │         name = convolve1         │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005d70    │ ╎  │
╎ │         name = merge_bn          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044003bf0    │ ╎  │
╎ │         name = convolve1         │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044005d70    │ ╎  │
╎ │         name = merge_bn          │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │      clEnqueueNDRangeKernel      │ ╎  │
╎ │    cl_kernel = 0x71c044006da0    │ ╎  │
╎ │           name = Xgemv           │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │        clEnqueueMapBuffer        │ ╎  │
╎ │         blocking = False         │ ╎  │
╎ │     buffer = 0x71c0440086c0      │ ╎  │
╎ │          map_flags = 1           │ ╎  │
╎ │            offset = 0            │ ╎  │
╎ │           size = 7432            │ ╎  │
╎ │   mapped_ptr = 0x71c045014480    │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │        clEnqueueMapBuffer        │ ╎  │
╎ │         blocking = False         │ ╎  │
╎ │     buffer = 0x71c0440089a0      │ ╎  │
╎ │          map_flags = 1           │ ╎  │
╎ │            offset = 0            │ ╎  │
╎ │            size = 512            │ ╎  │
╎ │   mapped_ptr = 0x71c045031580    │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎   │                                  ╎  │
╎   │                                  ╎  │
╎   ▼                                  ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │        clEnqueueMapBuffer        │ ╎  │
╎ │         blocking = False         │ ╎  │
╎ │     buffer = 0x71c044008c80      │ ╎  │
╎ │          map_flags = 1           │ ╎  │
╎ │            offset = 0            │ ╎  │
╎ │            size = 512            │ ╎  │
╎ │   mapped_ptr = 0x71c045033600    │ ╎  │
╎ └──────────────────────────────────┘ ╎  │
╎                                      ╎  │
└−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−┘  │
    │                                     │
    │                                     │
    ▼                                     │
┌−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−┐  │
╎       clReleaseCommandQueue()        ╎  │
╎                                      ╎  │
╎ ┌──────────────────────────────────┐ ╎  │
╎ │     clEnqueueUnmapMemObject      │ ╎  │
╎ │     memobj = 0x71c0440086c0      │ ╎  │
╎ │   mapped_ptr = 0x71c045014480    │ ╎ ◀┘
╎ └──────────────────────────────────┘ ╎
╎   │                                  ╎
╎   │                                  ╎
╎   ▼                                  ╎
╎ ┌──────────────────────────────────┐ ╎
╎ │     clEnqueueUnmapMemObject      │ ╎
╎ │     memobj = 0x71c0440089a0      │ ╎
╎ │   mapped_ptr = 0x71c045031580    │ ╎
╎ └──────────────────────────────────┘ ╎
╎   │                                  ╎
╎   │                                  ╎
╎   ▼                                  ╎
╎ ┌──────────────────────────────────┐ ╎
╎ │     clEnqueueUnmapMemObject      │ ╎
╎ │     memobj = 0x71c044008c80      │ ╎
╎ │   mapped_ptr = 0x71c045033600    │ ╎
╎ └──────────────────────────────────┘ ╎
╎                                      ╎
└−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−┘
```
