#ifdef _MSC_VER
#include <corecrt.h>
#endif

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/numpy.h>

#include <fmt/format.h>

#include "platform/sensors/BLEData.hpp"
#include "platform/io/py_BufferMessagePair.hpp"

namespace py = pybind11;

void platform_sensors_BLEData(py::module &m)
{
    py::class_<BLEData>(m, "BLEData")
        .def(py::init<>())
        .def_readonly_static("message_id", &BLEData::message_id)
        .def("serialize", &BLEData::serialize)
        .def("deserialize", &BLEData::deserialize)
        .def_readwrite("time", &BLEData::time_ms)
        .def_property("uuid",
            [](BLEData & data) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<uint8_t>::format());
                auto base = pybind11::array(dtype, {BLEData::uuid_len}, {sizeof(uint8_t)});
                return pybind11::array(
                    dtype, {BLEData::uuid_len}, {sizeof(uint8_t)}, data.uuid, base);
            },
            [](BLEData & data, pybind11::array_t<uint8_t> new_data)
            {
                if (new_data.size() != BLEData::uuid_len)
                    throw std::invalid_argument(fmt::format("Must set to {:d} element vector!", BLEData::uuid_len));
                for (auto i = 0; i < BLEData::uuid_len; ++i)
                    data.uuid[i] = new_data.at(i);
            })
        .def_readwrite("major", &BLEData::major)
        .def_readwrite("minor", &BLEData::minor)
        .def_readwrite("power", &BLEData::power)
        .def_readwrite("rssi", &BLEData::rssi)
        ;
    py::class_<BLEData::Buffer>(m, "BLEDataBuffer")
        .def(py::init<>())
        .def_property_readonly("valid_checksum", &BLEData::Buffer::valid_checksum)
        .def_property_readonly("buffer", [](const BLEData::Buffer & b) {
            return py::memoryview::from_memory(
                b.get_buffer(), b.used());
        })
        ;
    add_buffer_message_pair<BLEData>(m, "BLEMessage");
}
