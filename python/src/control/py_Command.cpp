#include <pybind11/pybind11.h>

#include "control/Command.hpp"
#include "platform/io/py_BufferMessagePair.hpp"

namespace py = pybind11;

void control_Command(py::module & m)
{
    py::enum_<Command::Commands>(m, "Commands")
        .value("NoOp", Command::Commands::NoOp)
        .export_values();
    py::class_<Command>(m, "Command")
        .def(py::init<>())
        .def_readonly_static("message_id", &Command::message_id)
        .def("serialize", &Command::serialize)
        .def("deserialize", &Command::deserialize)
        .def_readwrite("command", &Command::command)
        .def_readwrite("arg1", &Command::arg1)
        .def_readwrite("arg2", &Command::arg2)
        .def_readwrite("arg3", &Command::arg3)
        .def_readwrite("ack", &Command::ack)
        ;
    py::class_<Command::Buffer>(m, "CommandBuffer")
        .def(py::init<>())
        .def_property_readonly("valid_checksum", &Command::Buffer::valid_checksum)
        .def_property_readonly("buffer", [](const Command::Buffer & b) {
            return py::memoryview::from_memory(
                b.get_buffer(), b.used());
        })
        ;
    add_buffer_message_pair<Command>(m, "CommandMessage");
}
