#ifdef _MSC_VER
#include <corecrt.h>
#endif

#include <pybind11/pybind11.h>

#include "platform/io/MessageListener.hpp"

// Adding message support manually.
#include "tasks/TaskWaistTelemetry.hpp"
#include "tasks/TaskWatchTelemetry.hpp"

#include "control/Command.hpp"
#include "control/DataSummary.hpp"

#include "platform/sensors/FlashBlockData.hpp"

#include "platform/sensors/IMUData.hpp"
#include "platform/sensors/BLEData.hpp"

namespace py = pybind11;

typedef MessageListener<20, 300> PyMessageListener;

template <typename Td, typename Tm>
void add_support(Tm & c)
{
    c.def("add_listener", [](PyMessageListener & ml, typename Td::Buffer & buffer) {
        return ml.add_listener(Td::message_id, buffer.capacity(), &buffer.copy_callback);
    }).def("add_listener", [](PyMessageListener & ml, BufferMessagePair<Td> & message) {
        return ml.add_listener(message);
    }).def("write", [](PyMessageListener & ml, const typename Td::Buffer & buffer) {
        ml.write(buffer.get_buffer(), buffer.used());
    }).def("write", [](PyMessageListener & ml, const BufferMessagePair<Td> & message) {
        ml.write(message.buffer.get_buffer(), message.buffer.used());
    });
}

void platform_io_MessageListener(py::module &m)
{
    auto c = py::class_<PyMessageListener>(m, "MessageListener")
        .def(py::init<>())
        .def("listener_index", &PyMessageListener::listener_index)
        .def_property_readonly("num_messages", &PyMessageListener::num_messages)
        .def_property_readonly("num_ignored_messages", &PyMessageListener::num_ignored_messages)
        .def_property_readonly("num_skipped_bytes", &PyMessageListener::num_skipped_bytes)

        .def_property_readonly("capacity", &PyMessageListener::capacity)
        .def_property_readonly("used", &PyMessageListener::used)
        .def_property_readonly("free", &PyMessageListener::free)

        .def_property_readonly("buffer", [](const PyMessageListener & ml) {
            return py::memoryview::from_memory(
                ml.get_buffer(), ml.used());
        })

        .def("write", [](PyMessageListener & ml, const py::bytes & buffer) {
            auto info(py::buffer(buffer).request());
            auto data = reinterpret_cast<const uint8_t *>(info.ptr);
            uint16_t length = static_cast<uint16_t>(info.size);
            ml.write(data, length);
        })

        .def("process_next", &PyMessageListener::process_next)
        ;

    add_support<TaskWaistTelemetry>(c);
    add_support<TaskWatchTelemetry>(c);

    add_support<Command>(c);
    add_support<DataSummary>(c);

    add_support<FlashBlockData>(c);

    add_support<IMUData>(c);
    add_support<BLEData>(c);
}
