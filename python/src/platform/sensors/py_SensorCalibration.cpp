#ifdef _MSC_VER
    #include <corecrt.h>
#endif

#include <pybind11/pybind11.h>

#include "platform/sensors/SensorCalibration.hpp"

namespace py = pybind11;

template float SensorCalibration::scale<int>(int) const;
template float SensorCalibration::scale<double>(double) const;

void platform_sensors_SensorCalibration(py::module& m) {
    py::class_<SensorCalibration>(m, "SensorCalibration")
        .def(py::init<float, float>())
        .def(py::init<int, int, float, float>())
        .def("scale", &SensorCalibration::scale<int>)
        .def("scale", &SensorCalibration::scale<double>)
        .def_readwrite("m", &SensorCalibration::m)
        .def_readwrite("b", &SensorCalibration::b);
}
