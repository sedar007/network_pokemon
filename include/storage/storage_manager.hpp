#pragma once

namespace pokemon {
    class peer_registry;

    class NETWORK_POKEMON_STORAGE_API storage_manager {
        public:

            explicit storage_manager(std::string_view rootPath);

            [[nodiscard]] inline std::string_view get_path() const noexcept {
                return m_rootPath;
            }
            std::optional<Node_Info> loadNodeInfo() const;
            void saveNodeInfo(const Node_Info& info) const;
            std::vector<Node_Info> loadNodeList() const;
            void saveNodeList(const std::vector<Node_Info>& nodes) const;
            void addNodeToSavedList(const Node_Info& node);

            template <class P>
            void load_nodes(P& peers) noexcept {
                auto nodes = loadNodeList();
                for (const auto& nodeInfo : nodes) {
                    peers.add_node(nodeInfo);
                }
            }

            std::vector<Image> loadImageList() const;
            void saveImageList(const std::vector<Image>& images) const;
            void addImageToSavedList(const Image& image);

            template <class I>
            void load_images(I& image_repository) noexcept {
                auto images = loadImageList();
                for (const auto& image : images) {
                    image_repository.add_image(image);
                }
            }

            /* Image Cache */
            std::vector<image_cache> loadImageCacheList() const;
            void saveImageCacheList(const std::vector<image_cache>& images) const;
            void addImageCacheToSavedList(const image_cache& image);
            std::string get_image_cache_data(std::string_view hash) const;



    private:
        std::string_view m_rootPath;

        static constexpr std::string_view FILE_NODE_INFO = "node_infos.json";
        static constexpr std::string_view FILE_NODE_LIST = "node_list.json";
        static constexpr std::string_view FILE_IMAGE_LIST = "pokemons_images.json";
        static constexpr std::string_view FILE_IMAGE_CACHE_DATA = "image_cache_data.json";
    };


};

