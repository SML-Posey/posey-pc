#ifdef _MSC_VER
    #include <corecrt.h>
#endif

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include <fmt/format.h>

#include "control/DataSummary.hpp"
#include "platform/io/py_BufferMessagePair.hpp"

namespace py = pybind11;

void control_DataSummary(py::module& m) {
    py::class_<DataSummary>(m, "DataSummary")
        .def(py::init<>())
        .def_readonly_static("message_id", &DataSummary::message_id)
        .def("serialize", &DataSummary::serialize)
        .def("deserialize", &DataSummary::deserialize)
        .def_property(
            "datetime",
            [](DataSummary& data) -> pybind11::array {
                auto dtype = pybind11::dtype(
                    pybind11::format_descriptor<uint8_t>::format());
                auto base = pybind11::array(
                    dtype, {DataSummary::MaxDatetimeSize}, {sizeof(uint8_t)});
                return pybind11::array(
                    dtype, {DataSummary::MaxDatetimeSize}, {sizeof(uint8_t)},
                    data.datetime, base);
            },
            [](DataSummary& data, pybind11::array_t<uint8_t> new_data) {
                if (new_data.size() >= DataSummary::MaxDatetimeSize)
                    throw std::invalid_argument(fmt::format(
                        "Must set to max {:d} element vector!",
                        DataSummary::MaxDatetimeSize));
                for (auto i = 0; i < new_data.size(); ++i)
                    data.datetime[i] = new_data.at(i);
                for (auto i = new_data.size(); i < DataSummary::MaxDatetimeSize;
                     ++i)
                    data.datetime[i] = 0;
            })
        .def_readwrite("start_ms", &DataSummary::start_ms)
        .def_readwrite("end_ms", &DataSummary::end_ms)
        .def_readwrite("bytes", &DataSummary::bytes);
    py::class_<DataSummary::Buffer>(m, "DataSummaryBuffer")
        .def(py::init<>())
        .def_property_readonly(
            "valid_checksum", &DataSummary::Buffer::valid_checksum)
        .def_property_readonly("buffer", [](const DataSummary::Buffer& b) {
            return py::memoryview::from_memory(b.get_buffer(), b.used());
        });
    add_buffer_message_pair<DataSummary>(m, "DataSummaryMessage");
}
