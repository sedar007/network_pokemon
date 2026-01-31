#include "pch.h"

namespace pokemon {

    Test::Test() noexcept{
    }

    std::string Test::getIp() const noexcept {
        return "10.Sedar.0.0";
    }

    inline void to_json(nlohmann::json& j, const Config& c) {
        j = nlohmann::json{
                {"nodeName", c.nodeName},
                {"port", c.port},
                {"maxConnections", c.maxConnections},
                {"autoShare", c.autoShare},
                {"autoDownload", c.autoDownload}
        };
    }

    inline void from_json(const nlohmann::json& j, Config& c) {
        j.at("nodeName").get_to(c.nodeName);
        j.at("port").get_to(c.port);
        j.at("maxConnections").get_to(c.maxConnections);
        j.at("autoShare").get_to(c.autoShare);
        j.at("autoDownload").get_to(c.autoDownload);
    }

    void Test::saveConfig(std::string name, int port, int maxConn, bool share, bool download) {
        currentConfig = {name, port, maxConn, share, download};
        json::saveJson<Config>(storagePath, "config.json", currentConfig);
    }

    std::optional<Config> Test::getConfig() {
        auto config = json::loadJson<Config>(storagePath, "config.json");
        if (config.has_value()) {
            currentConfig = config.value();
            return currentConfig;
        }
        return std::nullopt;
    }

}
