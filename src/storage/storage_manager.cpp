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
        currentList.push_back(image);
        saveImageList(currentList);
    }

}
