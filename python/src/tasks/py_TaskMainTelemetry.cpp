#include <pybind11/pybind11.h>

#include "tasks/TaskMain.hpp"
#include "platform/io/py_BufferMessagePair.hpp"

namespace py = pybind11;

void tasks_TaskMainTelemetry(py::module & m)
{
    py::class_<TaskMainTelemetry>(m, "TaskMainTelemetry")
        .def(py::init<>())
        .def_readonly_static("message_id", &TaskMainTelemetry::message_id)
        .def("serialize", &TaskMainTelemetry::serialize)
        .def("deserialize", &TaskMainTelemetry::deserialize)
        .def_readwrite("counter", &TaskMainTelemetry::counter)
        .def_readwrite("t_start", &TaskMainTelemetry::t_start)
        .def_readwrite("t_end", &TaskMainTelemetry::t_end)
        .def_readwrite("invalid_checksum", &TaskMainTelemetry::invalid_checksum)
        .def_readwrite("missed_deadline", &TaskMainTelemetry::missed_deadline)
        ;
    py::class_<TaskMainTelemetry::Buffer>(m, "TaskMainTelemetryBuffer")
        .def(py::init<>())
        .def_property_readonly("valid_checksum", &TaskMainTelemetry::Buffer::valid_checksum)
        .def_property_readonly("buffer", [](const TaskMainTelemetry::Buffer & b) {
            return py::memoryview::from_memory(
                b.get_buffer(), b.used());
        })
        ;
    add_buffer_message_pair<TaskMainTelemetry>(m, "TaskMainTelemetryMessage");
}
