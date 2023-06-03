#ifdef _MSC_VER
#include <corecrt.h>
#endif

#include <pybind11/pybind11.h>

#include "tasks/TaskWaistTelemetry.hpp"
#include "platform/io/py_BufferMessagePair.hpp"

namespace py = pybind11;

void tasks_TaskWaistTelemetry(py::module & m)
{
    py::class_<TaskWaistTelemetry>(m, "TaskWaistTelemetry")
        .def(py::init<>())
        .def_readonly_static("message_id", &TaskWaistTelemetry::message_id)
        .def("serialize", &TaskWaistTelemetry::serialize)
        .def("deserialize", &TaskWaistTelemetry::deserialize)
        .def_readwrite("t_start", &TaskWaistTelemetry::t_start_ms)
        .def_readwrite("t_end", &TaskWaistTelemetry::t_end_ms)
        .def_readwrite("invalid_checksum", &TaskWaistTelemetry::invalid_checksum)
        .def_readwrite("missed_deadline", &TaskWaistTelemetry::missed_deadline)
        .def_readwrite("Vbatt", &TaskWaistTelemetry::Vbatt)
        .def_readwrite("ble_throughput", &TaskWaistTelemetry::ble_throughput)
        ;
    py::class_<TaskWaistTelemetry::Buffer>(m, "TaskWaistTelemetryBuffer")
        .def(py::init<>())
        .def_property_readonly("valid_checksum", &TaskWaistTelemetry::Buffer::valid_checksum)
        .def_property_readonly("buffer", [](const TaskWaistTelemetry::Buffer & b) {
            return py::memoryview::from_memory(
                b.get_buffer(), b.used());
        })
        ;
    add_buffer_message_pair<TaskWaistTelemetry>(m, "TaskWaistTelemetryMessage");
}
