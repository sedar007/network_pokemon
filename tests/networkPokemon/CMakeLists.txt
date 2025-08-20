find_package(boost_unit_test_framework CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}-tests
	"main.cpp"
	"modbus_master.test.cpp"
	"main.test.cpp"
)

target_link_libraries(${PROJECT_NAME}-tests
	PRIVATE 
		${PROJECT_NAME}
		Boost::unit_test_framework
)

add_test(${PROJECT_NAME}-Tests
	COMMAND 
		${PROJECT_NAME}-tests
)
