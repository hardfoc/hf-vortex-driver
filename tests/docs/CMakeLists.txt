# The code shown on the documentation pages: compiled as a library (warnings as errors) and run against the
# loopback transport by check_frames, which compares the bytes on the bus with the ones the pages show.
add_library(hf_vortex_doc_snippets OBJECT quickstart.cpp api_tasks.cpp)
target_link_libraries(hf_vortex_doc_snippets PRIVATE hf_vortex_driver)
target_compile_options(hf_vortex_doc_snippets PRIVATE -Wall -Wextra -Werror)

add_executable(hf_vortex_doc_check check_frames.cpp $<TARGET_OBJECTS:hf_vortex_doc_snippets>)
target_include_directories(hf_vortex_doc_check PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hf_vortex_doc_check PRIVATE hf_vortex_driver)
target_compile_options(hf_vortex_doc_check PRIVATE -Wall -Wextra -Werror)
add_test(NAME vortex_doc_frames COMMAND hf_vortex_doc_check)

# The BaseCan adapter on the porting page, when hf-internal-interface-wrap is available.
set(HF_VORTEX_BASECAN_INCLUDE_DIR "" CACHE PATH "hf-internal-interface-wrap/inc/base, to compile the BaseCan adapter")
if(HF_VORTEX_BASECAN_INCLUDE_DIR)
  add_library(hf_vortex_doc_basecan OBJECT basecan_transport.cpp)
  target_include_directories(hf_vortex_doc_basecan PRIVATE ${HF_VORTEX_BASECAN_INCLUDE_DIR})
  target_link_libraries(hf_vortex_doc_basecan PRIVATE hf_vortex_driver)
  target_compile_options(hf_vortex_doc_basecan PRIVATE -Wall -Wextra -Werror)
endif()
