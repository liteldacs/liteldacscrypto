cmake_minimum_required(VERSION 3.18)
project(piico-import C)

set(CMAKE_C_STANDARD 99)


option(SGW_DEVICE "SGW Device Option" OFF)


IF (SGW_DEVICE)
add_executable(${PROJECT_NAME} piico-import.c)
add_executable(cli::test_client ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC piico_ccmu)
INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /usr/local/bin)
ENDIF ()
