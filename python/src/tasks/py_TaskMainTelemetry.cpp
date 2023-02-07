#ifdef _MSC_VER
#include <corecrt.h>
#endif

#include <pybind11/pybind11.h>

#include "tasks/TaskTelemetry.hpp"
#include "platform/io/py_BufferMessagePair.hpp"

namespace py = pybind11;

void tasks_TaskTelemetry(py::module & m)
{
    py::class_<TaskTelemetry>(m, "TaskTelemetry")
        .def(py::init<>())
        .def_readonly_static("message_id", &TaskTelemetry::message_id)
        .def("serialize", &TaskTelemetry::serialize)
        .def("deserialize", &TaskTelemetry::deserialize)
        .def_readwrite("counter", &TaskTelemetry::counter)
        .def_readwrite("t_start", &TaskTelemetry::t_start)
        .def_readwrite("t_end", &TaskTelemetry::t_end)
        .def_readwrite("invalid_checksum", &TaskTelemetry::invalid_checksum)
        .def_readwrite("missed_deadline", &TaskTelemetry::missed_deadline)
        ;
    py::class_<TaskTelemetry::Buffer>(m, "TaskTelemetryBuffer")
        .def(py::init<>())
        .def_property_readonly("valid_checksum", &TaskTelemetry::Buffer::valid_checksum)
        .def_property_readonly("buffer", [](const TaskTelemetry::Buffer & b) {
            return py::memoryview::from_memory(
                b.get_buffer(), b.used());
        })
        ;
    add_buffer_message_pair<TaskTelemetry>(m, "TaskTelemetryMessage");
}
