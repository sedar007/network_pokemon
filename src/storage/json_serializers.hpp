#pragma once

namespace pokemon {

    // --- Node_Info Serialization ---
    inline void to_json(nlohmann::json& j, const Node_Info& n) {
        j = nlohmann::json{
                {Node_Info::NODE_ID_KEY, n.get_id()},
                {Node_Info::NODE_NAME_KEY, n.get_name()},
                {Node_Info::NODE_PORT_KEY, n.get_port()},
                {Node_Info::NODE_IP_KEY, n.get_ip()}
        };
    }

    inline void from_json(const nlohmann::json& j, Node_Info& n) {
        n.set_id(j.at(Node_Info::NODE_ID_KEY).get<std::string>());
        n.set_name(j.at(Node_Info::NODE_NAME_KEY).get<std::string>());
        n.set_port(j.at(Node_Info::NODE_PORT_KEY).get<int>());
        n.set_ip(j.at(Node_Info::NODE_IP_KEY).get<std::string>());
    }

    // --- Image Serialization ---
    inline void to_json(nlohmann::json& j, const Image& i) {
        j = nlohmann::json{
                {Image::IMAGE_NAME_KEY, i.get_name()},
                {Image::IMAGE_EXTENSION_KEY, i.get_extension()},
                {Image::IMAGE_HASH_KEY, i.get_hash()},
                {Image::IMAGE_OWNER_KEY, i.get_owner()},
                {Image::IMAGE_SIZE_KEY, i.get_size()},
                {Image::IMAGE_SIZE_UNIT_KEY, i.get_size_unit()}
        };
    }

    inline void from_json(const nlohmann::json& j, Image& i) {
        i.set_name(j.at(Image::IMAGE_NAME_KEY).get<std::string>());
        i.set_extension(j.at(Image::IMAGE_EXTENSION_KEY).get<std::string>());
        i.set_hash(j.at(Image::IMAGE_HASH_KEY).get<std::string>());
        i.set_owner(j.at(Image::IMAGE_OWNER_KEY).get<std::string>());
        i.set_size(j.at(Image::IMAGE_SIZE_KEY).get<std::string>());
        i.set_size_unit(j.at(Image::IMAGE_SIZE_UNIT_KEY).get<std::string>());
    }

    inline void to_json(nlohmann::json& j, const image_cache& i) {
        j = nlohmann::json{
                    {image_cache::IMAGE_CACHE_HASH_KEY, i.get_hash()},
                    {image_cache::IMAGE_CACHE_DATA_KEY, i.get_data()}
        };
    }

    inline void from_json(const nlohmann::json& j, image_cache& i) {
        i.set_hash(j.at(image_cache::IMAGE_CACHE_HASH_KEY).get<std::string>());
        i.set_data(j.at(image_cache::IMAGE_CACHE_DATA_KEY).get<std::string>());
    }
}
