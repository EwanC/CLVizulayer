# OpenCV

[OpenCV](https://opencv.org/) is an [open source](https://github.com/opencv/opencv)
computer vision library that features an [OpenCL backend](https://opencv.org/opencl)

The repository comes with examples that can be built with the `-DBUILD_EXAMPLES`
CMake flag. Using the [facedetect](https://github.com/opencv/opencv/blob/4.x/samples/cpp/facedetect.cpp)
sample with a stock JPEG image of a face, we can produce create a DOT graph of
the asynchronous OpenCL calls.

```sh
$ VIZ_VERBOSE=1 OPENCL_LAYERS=libCLVizuLayer.so ./bin/example_cpp_facedetect face.jpg
```

This produced the DOT output in [trace.dot](trace.dot) which can be rendered as
![opencv_facedetect_sample.svg](../../images/opencv_facedetect_sample.svg). It is
interesting to note the lack of kernel enqueues, and the only operation is blocking
buffer writes.
