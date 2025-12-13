cmake_minimum_required(VERSION 4.0)
project(GateServer)

set(CMAKE_CXX_STANDARD 14)

# boost
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/boost)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/boost/stage/lib)

# jsoncpp 头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/jsoncpp/libjson/include)

# GRPC 头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/third_party/re2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/third_party/address_sorting/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/third_party/abseil-cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/third_party/protobuf/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/include)

# HiRedis 头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/redis/redis/deps/hiredis)

# 添加所有 GRPC 库路径
set(GRPC_LIBRARY_PATHS
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/re2/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/types/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/synchronization/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/status/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/random/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/flags/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/debugging/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/container/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/hash/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/boringssl-with-bazel/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/numeric/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/time/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/base/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/abseil-cpp/absl/strings/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/protobuf/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/zlib/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/Debug"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/grpc/VisualStudio/third_party/cares/cares/lib/Debug"
)
link_directories(${GRPC_LIBRARY_PATHS})

# 添加 HiRedis 库路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/redis/redis/lib)

add_executable(${PROJECT_NAME}
        GateServer.cpp
        Cpp/CServer.cpp
        Headers/CServer.h
        Headers/const.h
        Cpp/HttpConnection.cpp
        Headers/HttpConnection.h
        Headers/Singleton.h
        Cpp/LogicSystem.cpp
        Headers/LogicSystem.h
        grpc/message.pb.h
        grpc/message.grpc.pb.cc
        grpc/message.pb.cc
        grpc/message.grpc.pb.h
        grpc/message.proto
        Cpp/VerifyGrpcClient.cpp
        Headers/VerifyGrpcClient.h
        Cpp/ConfigManager.cpp
        Headers/ConfigManager.h
        Cpp/AsioIOServicePool.cpp
        Headers/AsioIOServicePool.h
        Cpp/RedisManager.cpp
        Headers/RedisManager.h
        Cpp/RedisConnectionPool.cpp
        Headers/RedisConnectionPool.h
)

# 复制config.ini到输出目录
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/config.ini"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# jsoncpp lib库
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/jsoncpp/libjson/lib/json_vc71_libmtd.lib
)

# 添加到 grpc 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
        libprotobufd.lib
        gpr.lib
        grpc.lib
        grpc++.lib
        grpc++_reflection.lib
        address_sorting.lib
        ws2_32.lib
        cares.lib
        zlibstaticd.lib
        upb.lib
        ssl.lib
        crypto.lib
        absl_bad_any_cast_impl.lib
        absl_bad_optional_access.lib
        absl_bad_variant_access.lib
        absl_base.lib
        absl_city.lib
        absl_civil_time.lib
        absl_cord.lib
        absl_debugging_internal.lib
        absl_demangle_internal.lib
        absl_examine_stack.lib
        absl_exponential_biased.lib
        absl_failure_signal_handler.lib
        absl_flags.lib
        absl_flags_config.lib
        absl_flags_internal.lib
        absl_flags_marshalling.lib
        absl_flags_parse.lib
        absl_flags_program_name.lib
        absl_flags_usage.lib
        absl_flags_usage_internal.lib
        absl_graphcycles_internal.lib
        absl_hash.lib
        absl_hashtablez_sampler.lib
        absl_int128.lib
        absl_leak_check.lib
        absl_leak_check_disable.lib
        absl_log_severity.lib
        absl_malloc_internal.lib
        absl_periodic_sampler.lib
        absl_random_distributions.lib
        absl_random_internal_distribution_test_util.lib
        absl_random_internal_pool_urbg.lib
        absl_random_internal_randen.lib
        absl_random_internal_randen_hwaes.lib
        absl_random_internal_randen_hwaes_impl.lib
        absl_random_internal_randen_slow.lib
        absl_random_internal_seed_material.lib
        absl_random_seed_gen_exception.lib
        absl_random_seed_sequences.lib
        absl_raw_hash_set.lib
        absl_raw_logging_internal.lib
        absl_scoped_set_env.lib
        absl_spinlock_wait.lib
        absl_stacktrace.lib
        absl_status.lib
        absl_strings.lib
        absl_strings_internal.lib
        absl_str_format_internal.lib
        absl_symbolize.lib
        absl_synchronization.lib
        absl_throw_delegate.lib
        absl_time.lib
        absl_time_zone.lib
        absl_statusor.lib
        re2.lib
)

# HiRedis 连接库
target_link_libraries(${PROJECT_NAME} PRIVATE
        hiredis.lib
        Win32_Interop.lib
)