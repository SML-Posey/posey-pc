#ifdef _MSC_VER
    #include <corecrt.h>
#endif

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include <fmt/format.h>

#include "control/Command.hpp"
#include "platform/io/py_BufferMessagePair.hpp"

namespace py = pybind11;

void control_Command(py::module& m) {
    py::enum_<Command::CommandType>(m, "CommandType")
        .value("Configure", Command::Configure)
        .value("ConnectPeripheral", Command::ConnectPeripheral)
        .value("Reboot", Command::Reboot)
        .value("GetDataSummary", Command::GetDataSummary)
        .value("DownloadData", Command::DownloadData)
        .value("FullFlashErase", Command::FullFlashErase)
        .value("StartCollecting", Command::StartCollecting)
        .value("StopCollecting", Command::StopCollecting)
        .value("NoOp", Command::NoOp)
        .export_values();
    py::class_<Command>(m, "Command")
        .def(py::init<>())
        .def_readonly_static("message_id", &Command::message_id)
        .def("serialize", &Command::serialize)
        .def("deserialize", &Command::deserialize)
        .def("command_str", &Command::command_str)
        .def_readwrite("command", &Command::command)
        .def_property(
            "payload",
            [](Command& data) -> pybind11::array {
                auto dtype = pybind11::dtype(
                    pybind11::format_descriptor<uint8_t>::format());
                auto base = pybind11::array(
                    dtype, {Command::MaxPayload}, {sizeof(uint8_t)});
                return pybind11::array(
                    dtype, {Command::MaxPayload}, {sizeof(uint8_t)},
                    data.payload, base);
            },
            [](Command& data, pybind11::array_t<uint8_t> new_data) {
                if (new_data.size() >= Command::MaxPayload)
                    throw std::invalid_argument(fmt::format(
                        "Must set to max {:d} element vector!",
                        Command::MaxPayload));
                for (auto i = 0; i < new_data.size(); ++i)
                    data.payload[i] = new_data.at(i);
                for (auto i = new_data.size(); i < Command::MaxPayload; ++i)
                    data.payload[i] = 0;
            })
        .def_readwrite("ack", &Command::ack);
    py::class_<Command::Buffer>(m, "CommandBuffer")
        .def(py::init<>())
        .def_property_readonly(
            "valid_checksum", &Command::Buffer::valid_checksum)
        .def_property_readonly("buffer", [](const Command::Buffer& b) {
            return py::memoryview::from_memory(b.get_buffer(), b.used());
        });
    add_buffer_message_pair<Command>(m, "CommandMessage");
}
