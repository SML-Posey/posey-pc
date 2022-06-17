#ifdef _MSC_VER
#include <corecrt.h>
#endif

#include <pybind11/pybind11.h>

#include "platform/sensors/IMUData.hpp"
#include "platform/io/py_BufferMessagePair.hpp"

namespace py = pybind11;

void platform_sensors_IMUData(py::module &m)
{
    py::class_<IMUData>(m, "IMUData")
        .def(py::init<>())
        .def_readonly_static("message_id", &IMUData::message_id)
        .def("serialize", &IMUData::serialize)
        .def("deserialize", &IMUData::deserialize)
        .def_readwrite("time", &IMUData::time)
        .def_readwrite("An", &IMUData::An)
        .def_readwrite("Gn", &IMUData::Gn)
        .def_readwrite("Mn", &IMUData::Mn)
        .def_readwrite("Qn", &IMUData::Qn)
        .def_readwrite("Ax", &IMUData::Ax)
        .def_readwrite("Ay", &IMUData::Ay)
        .def_readwrite("Az", &IMUData::Az)
        .def_readwrite("Gx", &IMUData::Gx)
        .def_readwrite("Gy", &IMUData::Gy)
        .def_readwrite("Gz", &IMUData::Gz)
        .def_readwrite("Mx", &IMUData::Mx)
        .def_readwrite("My", &IMUData::My)
        .def_readwrite("Mz", &IMUData::Mz)
        .def_readwrite("Qi", &IMUData::Qi)
        .def_readwrite("Qj", &IMUData::Qj)
        .def_readwrite("Qk", &IMUData::Qk)
        .def_readwrite("Qr", &IMUData::Qr)
        .def_readwrite("Qacc", &IMUData::Qacc)
        ;
    py::class_<IMUData::Buffer>(m, "IMUDataBuffer")
        .def(py::init<>())
        .def_property_readonly("valid_checksum", &IMUData::Buffer::valid_checksum)
        .def_property_readonly("buffer", [](const IMUData::Buffer & b) {
            return py::memoryview::from_memory(
                b.get_buffer(), b.used());
        })
        ;
    add_buffer_message_pair<IMUData>(m, "IMUMessage");
}
