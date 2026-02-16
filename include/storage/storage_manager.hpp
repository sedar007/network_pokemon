#pragma once

namespace pokemon {

    class NETWORK_POKEMON_STORAGE_API storage_manager {
        public:

            explicit storage_manager(std::string_view rootPath);
            std::optional<Node_Info> loadNodeInfo() const;
            void saveNodeInfo(const Node_Info& info) const;

            std::vector<Node_Info> loadNodeList() const;
            void saveNodeList(const std::vector<Node_Info>& nodes) const;
            void addNodeToSavedList(const Node_Info& node);

            std::vector<Image> loadImageList() const;
            void saveImageList(const std::vector<Image>& images) const;
            void addImageToSavedList(const Image& image);

    private:
        std::string_view m_rootPath;

        static constexpr std::string_view FILE_NODE_INFO = "node_infos.json";
        static constexpr std::string_view FILE_NODE_LIST = "node_list.json";
        static constexpr std::string_view FILE_IMAGE_LIST = "pokemons_images.json";
    };


};

