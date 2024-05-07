#include <mutex>
#include "../hpp/trace.hpp"
#include "sstream"

namespace pokemon {
    Trace *Trace::trace = nullptr;
    std::mutex Trace::mutex;

    Trace &Trace::getInstance() {
        std::lock_guard<std::mutex> lock(mutex);

        if (!trace)
            trace = new Trace();
        return *trace;
    }

    void Trace::print(std::ostream &os, const std::string &msg) {
        std::lock_guard<std::mutex> lock(mutex);
        if (affiche)
            os << msg << std::endl;
    }

    void Trace::print(std::ostream &os, const std::string &codeAnsi, const std::string &msg) {
        std::lock_guard<std::mutex> lock(mutex);
        if (affiche)
            os << codeAnsi << msg << "\\e[0m" << std::endl;

    }


    void Trace::On() {
        affiche = true;
    }


}