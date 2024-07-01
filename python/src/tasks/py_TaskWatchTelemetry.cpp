#ifdef _MSC_VER
    #include <corecrt.h>
#endif

#include <pybind11/pybind11.h>

#include "platform/io/py_BufferMessagePair.hpp"
#include "tasks/TaskWatchTelemetry.hpp"

namespace py = pybind11;

void tasks_TaskWatchTelemetry(py::module& m) {
    py::class_<TaskWatchTelemetry>(m, "TaskWatchTelemetry")
        .def(py::init<>())
        .def_readonly_static("message_id", &TaskWatchTelemetry::message_id)
        .def("serialize", &TaskWatchTelemetry::serialize)
        .def("deserialize", &TaskWatchTelemetry::deserialize)
        .def_readwrite("t_start", &TaskWatchTelemetry::t_start_ms)
        .def_readwrite("t_end", &TaskWatchTelemetry::t_end_ms)
        .def_readwrite(
            "invalid_checksum", &TaskWatchTelemetry::invalid_checksum)
        .def_readwrite("missed_deadline", &TaskWatchTelemetry::missed_deadline)
        .def_readwrite("Vbatt", &TaskWatchTelemetry::Vbatt);
    py::class_<TaskWatchTelemetry::Buffer>(m, "TaskWatchTelemetryBuffer")
        .def(py::init<>())
        .def_property_readonly(
            "valid_checksum", &TaskWatchTelemetry::Buffer::valid_checksum)
        .def_property_readonly(
            "buffer", [](const TaskWatchTelemetry::Buffer& b) {
                return py::memoryview::from_memory(b.get_buffer(), b.used());
            });
    add_buffer_message_pair<TaskWatchTelemetry>(m, "TaskWatchTelemetryMessage");
}
