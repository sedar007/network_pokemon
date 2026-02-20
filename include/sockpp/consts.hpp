#pragma once
namespace pokemon::tcp {

    enum class PROTOCOL {
        GET_IPS, GET_PICS, GET_PIC, GET_ALIVE, GET_ID, UNKNOWN
    };
    static constexpr size_t PROTOCOL_SIZE = 8;


    inline  std::string protocolToString(const PROTOCOL q) noexcept {
        switch (q) {
        case PROTOCOL::GET_IPS:
            return "GET_IPS_";
        case PROTOCOL::GET_PICS:
            return "GET_PICS";
        case PROTOCOL::GET_PIC:
            return "GET_PIC_";
        case PROTOCOL::GET_ALIVE:
            return "GETALIVE";
        case PROTOCOL::GET_ID:
            return "GET_ID__";
        default:
            return "UNKNOWN_";
        }
    }

    inline PROTOCOL string_to_protocol(std::string_view s) noexcept {
        if (s == "GET_IPS_")
            return PROTOCOL::GET_IPS;
        if (s == "GETALIVE")
            return PROTOCOL::GET_ALIVE;
        if (s == "GET_ID__")
            return PROTOCOL::GET_ID;
        if (s == "GET_PICS")
            return PROTOCOL::GET_PICS;
        if (s=="GET_PIC_")
            return PROTOCOL::GET_PIC;
        return PROTOCOL::UNKNOWN;
    }

}
