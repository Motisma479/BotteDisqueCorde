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