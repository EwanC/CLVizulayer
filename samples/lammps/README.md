# LAMMPS

[LAMMPS](https://docs.lammps.org/Manual.html)
is a molecular dynamics simulation code focused on materials modeling with
has support for [GPU acceleration](https://docs.lammps.org/Build_extras.html#gpu)
with a variety of APIs including OpenCL.

The source code is [open source](https://github.com/lammps/lammps) and can be built with
`-DPKG_GPU=1` and `-DUSE_STATIC_OPENCL_LOADER=0` to enable GPU offloading through a dynamically
linked OpenCL ICD loader. Note that by default LAMMPS statically links to an ICD loader with
layers disabled, in order to use ICD layers we need to unset this default.

To test CLVizulayer we can use a LAMMPS sample, which are enabled in the build
with `-DBUILD_SAMPLES=1`. Greyon is the abstraction used in LAMMPS to wrap the
OpenCL API calls, by default is inserts many `clEnqueueMarker` call which make
the trace more complex, and can be disabled with
`-DCMAKE_CXX_FLAGS="-DGERYON_NO_OCL_MARKERS"`. This leads to a CMake call of:

```sh
$ cmake ../cmake -GNinja \
        -DPKG_GPU=1 \
        -DUSE_STATIC_OPENCL_LOADER=0 \
        -DCMAKE_CXX_FLAGS="-DGERYON_NO_OCL_MARKERS" \
        -DBUILD_SAMPLES=1
```

We can run the 2D [grid sample](https://github.com/lammps/lammps/tree/develop/examples/grid)
with OpenCL backend by using `-pk gpu 1 -sf gpu` in the LAMMPS invocation.

```sh
$ cd $LAMMPS_SOURCE/examples/grid
$ OPENCL_LAYERS=libCLVizuLayer.so ../../build/lmp -i in.grid.2d -pk gpu 1 -sf gpu
```

This produces the DOT output in [trace.dot](trace.dot) which can be rendered as
![lammps.svg](../../images/lammps.svg).
