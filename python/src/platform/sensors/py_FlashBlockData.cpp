#ifdef _MSC_VER
#include <corecrt.h>
#endif

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/numpy.h>

#include <fmt/format.h>

#include "platform/sensors/FlashBlockData.hpp"
#include "platform/io/py_BufferMessagePair.hpp"

namespace py = pybind11;

void platform_sensors_FlashBlockData(py::module &m)
{
    py::class_<FlashBlockData>(m, "FlashBlockData")
        .def(py::init<>())
        .def_readonly_static("message_id", &FlashBlockData::message_id)
        .def("serialize", &FlashBlockData::serialize)
        .def("deserialize", &FlashBlockData::deserialize)
        .def_readwrite("time", &FlashBlockData::time)
        .def_readwrite("slot", &FlashBlockData::slot)
        .def_readwrite("rssi", &FlashBlockData::rssi)
        .def_readwrite("block_bytes", &FlashBlockData::block_bytes)
        ;
    py::class_<FlashBlockData::Buffer>(m, "FlashBlockDataBuffer")
        .def(py::init<>())
        .def_property_readonly("valid_checksum", &FlashBlockData::Buffer::valid_checksum)
        .def_property_readonly("buffer", [](const FlashBlockData::Buffer & b) {
            return py::memoryview::from_memory(
                b.get_buffer(), b.used());
        })
        ;
    add_buffer_message_pair<FlashBlockData>(m, "FlashBlockMessage");
}
