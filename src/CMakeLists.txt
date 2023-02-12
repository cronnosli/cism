set(CISM_BIN_DIR ${CMAKE_BINARY_DIR}/bin)

add_executable(cism cism.cpp execute.cpp system_call.cpp)
target_link_libraries(cism nlohmann_json::nlohmann_json)
set_target_properties(cism PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CISM_BIN_DIR})

install(TARGETS cism)