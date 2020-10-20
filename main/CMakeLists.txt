set(COMPONENT_SRCS
        "main.cpp"
        "Config.cpp"
        "ModeOptionsPersister.cpp"

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
        "LedMode/ModeText.cpp"
        "LedMode/ModeTime.cpp"
        "LedMode/MatesDemo.cpp"
        "LedMode/Wave.cpp"
        "LedMode/FancyDemo.cpp"
        "LedMode/GameOfLife.cpp"
        )

if(CONFIG_ENABLE_CAM)
    set(COMPONENT_SRCS
            ${COMPONENT_SRCS}
            "LedMode/Camera.cpp"
            )
#    set(EXTRA_COMPONENT_DIRS ../../esp/components/esp32-camera)
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
            "mqtt/MqttAdapter.cpp"
            )
    message(STATUS "LedWall: MQTT Interface enabled")
else()
    message(STATUS "LedWall: MQTT Interface disabled")
endif()

if(CONFIG_LEDWALL_SENSORS)
    set(COMPONENT_SRCS
            ${COMPONENT_SRCS}
            "Sensors/Sensors.cpp"
            )
    message(STATUS "LedWall: Sensors enabled")
else()
    message(STATUS "LedWall: Sensors disabled")
endif()

register_component(
        SRCS ${COMPONENT_SRCS}
        INCLUDE_DIRS "."
)
