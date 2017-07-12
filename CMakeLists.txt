cmake_minimum_required(VERSION 3.8)
project(BS_LBF_c_version)

set(CMAKE_C_STANDARD 11)

file(COPY Localizedjpn.lbf
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(bs1_filetolbf bs1_filetolbf.c)