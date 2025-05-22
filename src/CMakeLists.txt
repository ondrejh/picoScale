option(BUILD_FOR_PICO "Build for Raspberry Pi Pico" ON)

cmake_minimum_required(VERSION 3.13)

# Define the project name (once)
set(PROJECT_NAME proj)

#target_include_directories(${PROJECT_NAME}
set(CJSON_PATH ${CMAKE_CURRENT_LIST_DIR}/../ext/cjson)


if (BUILD_FOR_PICO)
  # initialize the SDK based on PICO_SDK_PATH
  # note: this must happen before project()
  set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/../ext/pico-sdk)
  include(pico_sdk_import.cmake)

  # Set project name
  project(${PROJECT_NAME} C CXX ASM)
  set(CMAKE_C_STANDARD 11)
  set(CMAKE_CXX_STANDARD 17)

  # initialize the Raspberry Pi Pico SDK
  pico_sdk_init()

  # rest of your project
  add_executable(${PROJECT_NAME}
    main.c
    data.c
    utils.c
    ws2812.c
    ws2812.pio
    hx711.c
  )

  pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/../ext/pico-examples/pio/ws2812/ws2812.pio)

  # add external dirrectoris (for headers)
  target_include_directories(${PROJECT_NAME} PRIVATE
    ${CJSON_PATH}
  )

  # add sources
  target_sources(${PROJECT_NAME} PRIVATE
    ${CJSON_PATH}/cJSON.c
  )

  # add pico_stdlib library which aggregates commonly used features
  target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_pio
  )

  # Enable USB output, disable UART if desired
  pico_enable_stdio_usb(${PROJECT_NAME} 1)
  pico_enable_stdio_uart(${PROJECT_NAME} 0)

  # create map/bin/hex/uf2 file in addition to ELF.
  pico_add_extra_outputs(${PROJECT_NAME})

else()

  project(${PROJECT_NAME})

  set(SOURCES main.c data.c utils.c ${CJSON_PATH}/cJSON.c)

  add_executable(${PROJECT_NAME} ${SOURCES})

  target_compile_definitions(${PROJECT_NAME} PRIVATE SIMUL)

  target_include_directories(${PROJECT_NAME} PRIVATE ${CJSON_PATH})

  #set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES)

endif()
