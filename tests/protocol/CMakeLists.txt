# Host unit tests of the protocol constants and their wire encoding.
add_executable(hf_vortex_protocol_test protocol_test.cpp)
target_link_libraries(hf_vortex_protocol_test PRIVATE hf_vortex_driver)
target_compile_options(hf_vortex_protocol_test PRIVATE -Wall -Wextra -Werror)
add_test(NAME vortex_protocol COMMAND hf_vortex_protocol_test)
