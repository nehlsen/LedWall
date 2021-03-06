set(COMPONENT_SRCS
        "main.cpp"
        "Config.cpp"
        "ModeOptionsPersister.cpp"

        "PresetManager/PresetManager.cpp"
        "PresetManager/Preset.cpp"

        "ModeController.cpp"
        "LedMode/LedMode.cpp"
        "LedMode/LedModes.cpp"
        "LedMode/utilities.c"
        "LedMode/TimeFormatter.cpp"

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
        "LedMode/FancyDemo/FancyDemoPart.cpp"
        "LedMode/FancyDemo/FancyDemoPart3.cpp"
        "LedMode/FancyDemo/FancyDemoPart4.cpp"
        "LedMode/FancyDemo/FdBarsGrow.cpp"
        "LedMode/FancyDemo/FdBarsOpen.cpp"
        "LedMode/FancyDemo/FdCircleGrow.cpp"
        "LedMode/FancyDemo/FdColorRays.cpp"
        "LedMode/FancyDemo/FdCubeGrow.cpp"
        "LedMode/FancyDemo/FdCubeRoll.cpp"
        "LedMode/FancyDemo/FdExplodingLetters.cpp"
        "LedMode/FancyDemo/FdNHeartC.cpp"
        "LedMode/FancyDemo/FdSprinkle.cpp"
        "LedMode/FancyDemo/Nothing.cpp"
        "LedMode/GameOfLife.cpp"
        )

if(CONFIG_ENABLE_CAM)
    list(APPEND COMPONENT_SRCS LedMode/Camera.cpp)
#    set(EXTRA_COMPONENT_DIRS ../../esp/components/esp32-camera)
    message(STATUS "LedWall: CAM Driver enabled")
else()
    message(STATUS "LedWall: CAM Driver disabled")
endif()

if(CONFIG_ENABLE_REST)
    list(APPEND COMPONENT_SRCS http/Power.cpp http/Mode.cpp http/ModeOptions.cpp http/ModeList.cpp http/Preset.cpp)
    message(STATUS "LedWall: REST Interface enabled")
else()
    message(STATUS "LedWall: REST Interface disabled")
endif()

if(CONFIG_ENABLE_MQTT)
    list(APPEND COMPONENT_SRCS mqtt/MqttAdapter.cpp)
    message(STATUS "LedWall: MQTT Interface enabled")
else()
    message(STATUS "LedWall: MQTT Interface disabled")
endif()

register_component(
        SRCS ${COMPONENT_SRCS}
        INCLUDE_DIRS "."
)
