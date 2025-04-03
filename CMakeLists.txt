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
    "Sources/Commands/Invite.cpp"
    "Sources/Commands/Meme.cpp"
    "Sources/Commands/Ping.cpp"
    "Sources/Commands/Reload.cpp"
    "Sources/Commands/Stop.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(Externals/DPP)
target_link_libraries(${PROJECT_NAME} dpp)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE_DIR:dpp>/dpp.dll
        $<TARGET_FILE_DIR:dpp>/libssl-1_1-x64.dll
        $<TARGET_FILE_DIR:dpp>/libcrypto-1_1-x64.dll
        $<TARGET_FILE_DIR:dpp>/zlib1.dll
        $<TARGET_FILE_DIR:dpp>/opus.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)