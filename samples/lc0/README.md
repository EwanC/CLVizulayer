# Leela Chess Zero

[Leela Chess Zero](https://lczero.org/) or "lc0" is an [open source](https://github.com/LeelaChessZero/lc0)
AI chess engine that supports multiple backends, one of which is OpenCL.

CLVizulayer to produce a DOT graph for the OpenCL backend by using the
[T78 network](http://training.lczero.org/get_network?sha=d0ed346c32fbcc9eb2f0bc7e957d188c8ae428ee3ef7291fd5aa045fc6ef4ded)
from the [Best Nets for Lc0 documentation](https://lczero.org/dev/wiki/best-nets-for-lc0). This was
run on single board position for 1 node to produce a linear graph of device tasks.

```
$ OPENCL_LAYERS=libCLVizuLayer.so VIZ_VERBOSE=1  ./build/release/lc0 benchmark -w weights/T78 --num-positions=1 --nodes=1 --backend=opencl

       _
|   _ | |
|_ |_ |_| v0.31.2 built v0.33.0-dev+git.dirty built Apr 21 2026
Loading weights file from: weights/T78
OpenCL, maximum batch size set to 16.
Initializing OpenCL.
Detected 2 OpenCL platforms.
Platform version: OpenCL 3.0 CUDA 13.0.94
Platform profile: FULL_PROFILE
Platform name:    NVIDIA CUDA
Platform vendor:  NVIDIA Corporation
Device ID:      0
Device name:    NVIDIA RTX 500 Ada Generation Laptop GPU
Device type:    GPU
Device vendor:  NVIDIA Corporation
Device driver:  580.95.05
Device speed:   2025 MHZ
Device cores:   16 CU
Device score:   1130
Platform version: OpenCL 3.0 LINUX
Platform profile: FULL_PROFILE
Platform name:    Intel(R) OpenCL
Platform vendor:  Intel(R) Corporation
Device ID:      1
Device name:    Intel(R) Core(TM) Ultra 7 155H
Device type:    CPU
Device vendor:  Intel(R) Corporation
Device driver:  2026.20.1.0.12_160000
Device speed:   0 MHZ
Device cores:   22 CU
Device score:   2530
Selected platform: Intel(R) OpenCL
Selected device: Intel(R) Core(TM) Ultra 7 155H
with OpenCL 3.0 capability.
Loaded existing SGEMM tuning for batch size 16.
Wavefront/Warp size: 128

Graph Capture enabled: 0
Max workgroup size: 8192
Max workgroup dimensions: 8192 8192 8192

Position: 1/1 rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
Benchmark time 196 ms, 1 nodes, -1 nps, move e2e4
bestmove e2e4

===========================
Total time (ms) : 197
Nodes searched  : 1
Nodes/second    : 5
```

This creates the [trace.dot](trace.dot) trace, which can be rendered in unicode boxart with
`graph-easy trace.dot -as boxart` as [original.txt](original.txt).

## Command-buffer Modifications

By looking at the graph produced you can see a large block of kernels which execute without interruption. This
is an interesting candidate for modification to use the `cl_khr_command_buffer` OpenCL extension, which I
created a branch for in https://github.com/EwanC/lc0/pull/1

### Queue DOT Graph

With the command-buffer lc0 modifications we can run the application and see the effects on the queue submissions.

```sh
$ OPENCL_LAYERS=libCLVizuLayer.so VIZ_VERBOSE=1  ./build/release/lc0 benchmark -w weights/T78 --num-positions=1 --nodes=1 --backend=opencl --backend-opts=graph_capture=true
```

The trace produced can be seen in [modified.dot](modified.dot) with a boxart rendering in [modified.txt](modified.txt).

### Command-buffer DOT Graph

Using the `cl_ext_command_buffer_dot_print` extension we can also print the dot graph of the command-buffer itself.

```sh
$ OPENCL_LAYERS=libCLVizuLayer.so VIZ_EXT=1  ./build/debug/lc0 benchmark -w weights/T78 --num-positions=1 --nodes=1 --backend=opencl --backend-opts=graph_capture=true
```

Looking at the created graph for batch size 1 (only size run in this example) we see [graph_0.dot](graph_0.dot) which
is rendered in boxart as [graph.txt](graph.txt).


### Performance Impact

Intel CPU OpenCL implementation has native `cl_khr_command_buffer` extension support rather than emulated.

> TODO: results from graph capture on vs off.
