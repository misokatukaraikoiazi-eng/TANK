cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(tank_project)


# ビルド完了後に自動で特定のポートへ書き込み（フラッシュ）まで連動する設定
add_custom_command(
    TARGET app
    POST_BUILD
    # 1. まずは1本化バイナリを生成
    COMMAND esptool.py --chip esp32 merge_bin -o ${CMAKE_SOURCE_DIR}/target_production.bin @flash_args
#    # 2. ポートを指定して一発書き込み（-p で接続先ポートを明示）
#    COMMAND esptool.py --chip esp32 -p /dev/bus/usb/001/002 -b 460800 --before default_reset --after hard_reset write_flash 0x0 ${CMAKE_SOURCE_DIR}/target_production.bin
#    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#    COMMENT "Generating single binary and FLASHING to /dev/bus/usb/001/002 automatically..."
)
