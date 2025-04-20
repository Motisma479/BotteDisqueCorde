cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)

project(BotteDisqueCorde)

include_directories("Headers")

set(SOURCES
    "Sources/main.cpp"
    "Sources/Initialiser.cpp"
    "Sources/advancedFunction.cpp"

    "Sources/Commands/Amogus.cpp"
    "Sources/Commands/Clear.cpp"
    "Sources/Commands/Dice.cpp"
    "Sources/Commands/Dm.cpp"
    "Sources/Commands/Invite.cpp"
    "Sources/Commands/Meme.cpp"
    "Sources/Commands/Ping.cpp"
    "Sources/Commands/Reload.cpp"
    "Sources/Commands/Say.cpp"
    "Sources/Commands/Stop.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(Externals/DPP)
target_link_libraries(${PROJECT_NAME} dpp)

# copy all fille nedded to launch
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/settings.ini
        ${CMAKE_SOURCE_DIR}/superAdminId
        ${CMAKE_SOURCE_DIR}/token.ini
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
)

# copy all required dll
get_target_property(DPP_OUTPUT_DIR dpp BINARY_DIR)
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE_DIR:dpp>/dpp.dll
            ${DPP_OUTPUT_DIR}/libssl-1_1-x64.dll
            ${DPP_OUTPUT_DIR}/libcrypto-1_1-x64.dll
            ${DPP_OUTPUT_DIR}/zlib1.dll
            ${DPP_OUTPUT_DIR}/opus.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
else()
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE_DIR:dpp>/libdpp.so
            $<TARGET_FILE_DIR:dpp>/libdpp.so.10.1.1
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()
