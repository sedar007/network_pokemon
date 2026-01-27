find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}-exemple
        "main.cpp"
)

target_link_libraries(${PROJECT_NAME}-exemple
        PRIVATE
        ${PROJECT_NAME}-network-pokemon

)
