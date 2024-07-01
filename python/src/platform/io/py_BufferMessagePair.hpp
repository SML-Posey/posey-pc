#pragma once

#include <pybind11/pybind11.h>
#include <string>

#include "platform/io/BufferMessagePair.hpp"

template <typename Td>
void add_buffer_message_pair(pybind11::module& m, const char* const name) {
    namespace py = pybind11;
    py::class_<BufferMessagePair<Td>>(m, name)
        .def(py::init<>())
        .def_property_readonly(
            "valid_checksum", &BufferMessagePair<Td>::valid_checksum)
        .def("deserialize", &BufferMessagePair<Td>::deserialize)
        .def("serialize", &BufferMessagePair<Td>::serialize)
        .def_readonly("buffer", &BufferMessagePair<Td>::buffer)
        .def_readonly("message", &BufferMessagePair<Td>::message);
}
