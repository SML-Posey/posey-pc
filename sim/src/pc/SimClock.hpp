#pragma once

#include <chrono>
#include <thread>

class SimClock {
    public:
        static auto start() {
            static bool initialized = false;
            static std::chrono::high_resolution_clock::time_point start;
            if (!initialized) {
                start = std::chrono::high_resolution_clock::now();
                initialized = true;
            }
            return start;
        }

        template <class T>
        static void set_usec(T& time) {
            auto elapsed = std::chrono::high_resolution_clock::now() - start();
            time =
                std::chrono::duration_cast<std::chrono::microseconds>(elapsed)
                    .count();
        }

        template <class T>
        static void set_msec(T& time) {
            auto elapsed = std::chrono::high_resolution_clock::now() - start();
            time =
                std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)
                    .count();
        }

        template <class T>
        static T get_usec() {
            T usec;
            set_usec(usec);
            return usec;
        }

        template <class T>
        static T get_msec() {
            T msec;
            set_msec(msec);
            return msec;
        }

        template <class T>
        static void delay_usec(const T delay) {
            using namespace std::this_thread;
            using namespace std::chrono;

            sleep_for(microseconds(delay));
        }

        template <class T>
        static void delay_msec(const T delay) {
            using namespace std::this_thread;
            using namespace std::chrono;

            sleep_for(milliseconds(delay));
        }
};
