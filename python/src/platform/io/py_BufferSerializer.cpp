#ifdef _MSC_VER
#include <corecrt.h>
#endif

#include <pybind11/pybind11.h>

#include "platform/io/BufferSerializer.hpp"

namespace py = pybind11;

void platform_io_BufferSerializer(py::module &m)
{
    py::class_<BufferCopyCallback>(m, "BufferCopyCallback")
        ;
}
