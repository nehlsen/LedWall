set(COMPONENT_SRCS
        "main.cpp"
        "wifi_provisioning.c"
        "ConfigManager.cpp"
        "OtaUpdater.cpp"

        "ModeController.cpp"
        "LedMode/LedMode.cpp"
        "LedMode/LedModes.cpp"
        "LedMode/utilities.c"

        "LedMode/LedModeStatus.cpp"
        "LedMode/LedModeSample.cpp"
        "LedMode/LedModeHsiboy.cpp"
        "LedMode/Fireworks.cpp"
        "LedMode/Bars.cpp"
        "LedMode/MultiBars.cpp"
        "LedMode/Fire.cpp"
        "LedMode/Network.cpp"
        "LedMode/Breathe.cpp"
        "LedMode/Text.cpp"
        "LedMode/MatesDemo.cpp"
        )

if(CONFIG_ENABLE_CAM)
    set(COMPONENT_SRCS
            ${COMPONENT_SRCS}
            "LedMode/Camera.cpp"
            )
    set(EXTRA_COMPONENT_DIRS ../components/esp32-camera)
    message(STATUS "LedWall: CAM Driver enabled")
else()
    message(STATUS "LedWall: CAM Driver disabled")
endif()

if(CONFIG_ENABLE_REST)
    set(COMPONENT_SRCS
            ${COMPONENT_SRCS}
            "WebServer.cpp"
            "WebServer/FileResponseHandler.cpp"
            )
    message(STATUS "LedWall: REST Interface enabled")
else()
    message(STATUS "LedWall: REST Interface disabled")
endif()

if(CONFIG_ENABLE_MQTT)
    set(COMPONENT_SRCS
            ${COMPONENT_SRCS}
            "mqtt/Mqtt.cpp"
            )
    message(STATUS "LedWall: MQTT Interface enabled")
else()
    message(STATUS "LedWall: MQTT Interface disabled")
endif()

register_component(
        SRCS ${COMPONENT_SRCS}
        INCLUDE_DIRS "."
)

set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../data")
spiffs_create_partition_image(data ${DATA_DIR} FLASH_IN_PROJECT)
