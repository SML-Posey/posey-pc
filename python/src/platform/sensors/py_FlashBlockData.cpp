#ifdef _MSC_VER
    #include <corecrt.h>
#endif

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include <fmt/format.h>

#include "platform/io/py_BufferMessagePair.hpp"
#include "platform/sensors/FlashBlockData.hpp"

namespace py = pybind11;

void platform_sensors_FlashBlockData(py::module& m) {
    py::class_<FlashBlockData>(m, "FlashBlockData")
        .def(py::init<>())
        .def_readonly_static("message_id", &FlashBlockData::message_id)
        .def("serialize", &FlashBlockData::serialize)
        .def("deserialize", &FlashBlockData::deserialize)
        .def_readwrite("time", &FlashBlockData::time_ms)
        .def_readwrite("slot", &FlashBlockData::slot)
        .def_property(
            "mac",
            [](FlashBlockData& data) -> pybind11::array {
                auto dtype = pybind11::dtype(
                    pybind11::format_descriptor<uint8_t>::format());
                auto base = pybind11::array(dtype, {6}, {sizeof(uint8_t)});
                return pybind11::array(
                    dtype, {6}, {sizeof(uint8_t)}, data.mac, base);
            },
            [](FlashBlockData& data, pybind11::array_t<uint8_t> new_data) {
                if (new_data.size() != 6)
                    throw std::invalid_argument(
                        fmt::format("Must set MAC to {:d} element vector!", 6));
                for (auto i = 0; i < 6; ++i)
                    data.mac[i] = new_data.at(i);
            })
        .def_readwrite("rssi", &FlashBlockData::rssi)
        .def_readwrite("block_bytes", &FlashBlockData::block_bytes);
    py::class_<FlashBlockData::Buffer>(m, "FlashBlockDataBuffer")
        .def(py::init<>())
        .def_property_readonly(
            "valid_checksum", &FlashBlockData::Buffer::valid_checksum)
        .def_property_readonly(
            "buffer",
            [](const FlashBlockData::Buffer& b) {
                return py::memoryview::from_memory(b.get_buffer(), b.used());
            })
        .def(
            "write",
            [](FlashBlockData::Buffer& b, pybind11::array_t<uint8_t> new_data) {
                uint16_t size = new_data.size();
                if (size > b.capacity())
                    size = b.capacity();
                b.reset();
                b.write(new_data.data(), size);
            });
    add_buffer_message_pair<FlashBlockData>(m, "FlashBlockMessage");
}
