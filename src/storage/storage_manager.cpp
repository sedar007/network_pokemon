#include "pch.h"

namespace pokemon {

    storage_manager::storage_manager(std::string_view rootPath)
        : m_rootPath(rootPath) {}

    std::optional<Node_Info> storage_manager::loadNodeInfo() const {
        return Json::loadJson<Node_Info>(m_rootPath, FILE_NODE_INFO);
    }

    void storage_manager::saveNodeInfo(const Node_Info& info) const {
        Json::saveJson<Node_Info>(m_rootPath, FILE_NODE_INFO, info);
    }

    std::vector<Node_Info> storage_manager::loadNodeList() const {
        auto result = Json::loadJson<std::vector<Node_Info>>(m_rootPath, FILE_NODE_LIST);
        return result.value_or(std::vector<Node_Info>{});
    }

    void storage_manager::saveNodeList(const std::vector<Node_Info>& nodes) const {
        Json::saveJson<std::vector<Node_Info>>(m_rootPath, FILE_NODE_LIST, nodes);
    }

    void storage_manager::addNodeToSavedList(const Node_Info& node_info) {
        auto currentList = loadNodeList();

        auto it = std::find_if(currentList.begin(), currentList.end(),
            [&](const Node_Info& n){ return n.get_id() == node_info.get_id(); });
        if (it != currentList.end()) {
            return;
        }

        currentList.push_back(node_info);
        saveNodeList(currentList);
    }


    std::vector<Image> storage_manager::loadImageList() const {
        auto result = Json::loadJson<std::vector<Image>>(m_rootPath, FILE_IMAGE_LIST);
        return result.value_or(std::vector<Image>{});
    }

    void storage_manager::saveImageList(const std::vector<Image>& images) const {
        Json::saveJson<std::vector<Image>>(m_rootPath, FILE_IMAGE_LIST, images);
    }

    void storage_manager::addImageToSavedList(const Image& image) {
        auto currentList = loadImageList();

        auto it = std::find_if(currentList.begin(), currentList.end(),
            [&](const Image& img){ return img.get_hash() == image.get_hash(); });

        if (it != currentList.end()) {
            return;
        }

        currentList.push_back(image);
        saveImageList(currentList);
    }


    /* Image Cache */

    std::vector<image_cache> storage_manager::loadImageCacheList() const {
        auto result = Json::loadJson<std::vector<image_cache>>(m_rootPath, FILE_IMAGE_CACHE_DATA);
        return result.value_or(std::vector<image_cache>{});
    }

    void storage_manager::saveImageCacheList(const std::vector<image_cache>& images) const {
        Json::saveJson<std::vector<image_cache>>(m_rootPath, FILE_IMAGE_CACHE_DATA, images);
    }

    void storage_manager::addImageCacheToSavedList(const image_cache& image) {
        auto currentList = loadImageCacheList();

        auto it = std::find_if(currentList.begin(), currentList.end(),
            [&](const image_cache& img){ return img.get_hash() == image.get_hash(); });

        if (it != currentList.end()) {
            return;
        }

        currentList.push_back(image);
        saveImageCacheList(currentList);
    }

    std::string storage_manager::get_image_cache_data(std::string_view hash) const {
        auto cacheList = loadImageCacheList();
        auto it = std::find_if(cacheList.begin(), cacheList.end(),
            [&](const image_cache& img){ return img.get_hash() == hash; });

        if (it != cacheList.end()) {
            return it->get_data().data();
        }
        return "";
    }

}
