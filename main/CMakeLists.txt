set(COMPONENT_SRCS
        "main.cpp"
        "wifi_connector.c"
        "mqtt.cpp"
        "LedController.cpp"
        "LedMode/LedMode.cpp"
        "LedMode/LedModeStatus.cpp"
        "LedMode/LedModeSample.cpp"
        "LedMode/LedModeHsiboy.cpp"
        "LedMode/LedModes.cpp"
        "WebServer.cpp"
        "ConfigManager.cpp"
        )

register_component(
        SRCS ${COMPONENT_SRCS}
        INCLUDE_DIRS "."
)

set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../front")
if(EXISTS ${WEB_SRC_DIR}/dist)
    spiffs_create_partition_image(www ${WEB_SRC_DIR}/dist FLASH_IN_PROJECT)
else()
    message(FATAL_ERROR "${WEB_SRC_DIR}/dist doesn't exit. Please run 'npm run build' in ${WEB_SRC_DIR}")
endif()
