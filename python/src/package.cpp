#ifdef _MSC_VER
    #include <corecrt.h>
#endif

#include <pybind11/pybind11.h>

#include "MessageAck.hpp"

enum PyMessageAck {
    DontCare = MessageAck::DontCare,
    Expected = MessageAck::Expected,
    OK = MessageAck::OK,
    Resend = MessageAck::Resend,
    Error = MessageAck::Error,
    Working = MessageAck::Working
};

namespace py = pybind11;

void tasks_TaskWaistTelemetry(py::module& m);
void tasks_TaskWatchTelemetry(py::module& m);

void control_Command(py::module& m);
void control_DataSummary(py::module& m);

void platform_io_BufferSerializer(py::module& m);
void platform_io_MessageListener(py::module& m);

void platform_sensors_SensorCalibration(py::module& m);
void platform_sensors_IMUData(py::module& m);
void platform_sensors_BLEData(py::module& m);
void platform_sensors_FlashBlockData(py::module& m);

// TODO: Currently message buffer objects are aliased types defined by the
// message size, e.g., IMUData::Buffer == BufferSerializer<sizeof(IMUData)>.
// If two messages have the same size, they are (to pybind11) the same object,
// and you'll get an obscure error when you try to import the module that
// a type was already registered. So we need to add slop bytes to some messages
// to make sure they're all unique message sizes. :-/

PYBIND11_MODULE(pyposey, m) {
    py::enum_<PyMessageAck>(m, "MessageAck")
        .value("DontCare", PyMessageAck::DontCare)
        .value("Expected", PyMessageAck::Expected)
        .value("OK", PyMessageAck::OK)
        .value("Resend", PyMessageAck::Resend)
        .value("Error", PyMessageAck::Error)
        .value("Working", PyMessageAck::Working)
        .export_values();

    auto platform = m.def_submodule("platform");
    auto platform_io = platform.def_submodule("io");
    auto platform_sensors = platform.def_submodule("sensors");
    auto platform_hardware = platform.def_submodule("hardware");
    auto tasks = m.def_submodule("tasks");
    auto control = m.def_submodule("control");

    platform_io_BufferSerializer(platform_io);
    platform_io_MessageListener(platform_io);

    platform_sensors_SensorCalibration(platform_sensors);
    platform_sensors_IMUData(platform_sensors);
    platform_sensors_BLEData(platform_sensors);
    platform_sensors_FlashBlockData(platform_sensors);

    tasks_TaskWaistTelemetry(tasks);
    tasks_TaskWatchTelemetry(tasks);

    control_Command(control);
    control_DataSummary(control);

    m.attr("__version__") = "1.0";
}
