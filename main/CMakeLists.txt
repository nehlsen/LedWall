set(COMPONENT_SRCS
        "main.cpp"
        "wifi_connector.c"
        "LedController.cpp"
        "LedMode/LedMode.cpp"
        "LedMode/LedModeStatus.cpp"
        "LedMode/LedModeSample.cpp"
        "LedMode/LedModeHsiboy.cpp"
        "LedMode/LedModes.cpp"
        "LedMode/Fireworks.cpp"
        "ConfigManager.cpp"
        )

if(DEFINED FEATURE_REST)
    set(COMPONENT_SRCS
            ${COMPONENT_SRCS}
            "WebServer.cpp"
            "WebServer/FileResponseHandler.cpp"
            )
    add_definitions(-DFEATURE_REST)
endif()

if(DEFINED FEATURE_MQTT)
    set(COMPONENT_SRCS
            ${COMPONENT_SRCS}
            "mqtt.cpp"
            )
    add_definitions(-DFEATURE_MQTT)
endif()

register_component(
        SRCS ${COMPONENT_SRCS}
        INCLUDE_DIRS "."
)
