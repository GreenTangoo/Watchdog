cmake_minimum_required(VERSION 3.1)

project(Watchdog)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)

set(AGGREGATION_MODULE
    aggregation_module/aggregation_module_initializer.hpp
    aggregation_module/aggregation_module_initializer.cpp
    aggregation_module/aggregation_serializer_visitor.hpp
    aggregation_module/aggregation_serializer_visitor.cpp
    aggregation_module/grabber_base.hpp
    aggregation_module/grabber_base.cpp
    aggregation_module/net_grabbers.hpp
    aggregation_module/net_grabbers.cpp
    aggregation_module/system_grabbers.hpp
    aggregation_module/system_grabbers.cpp
)

set(CORRELATION_MODULE
    correlation_module/correlation_module_initializer.hpp
    correlation_module/correlation_module_initializer.cpp
    correlation_module/symptom_checking.hpp
    correlation_module/symptom_checking.cpp
    correlation_module/symptom_ssh_checking.hpp
    correlation_module/symptom_ssh_checking.cpp
    correlation_module/correlation/correlation.hpp
    correlation_module/correlation/correlation.cpp
    correlation_module/correlation/machine_learning/correlation_ml.hpp
    correlation_module/correlation/machine_learning/correlation_ml.cpp
    correlation_module/correlation/python_invoke.hpp
    correlation_module/correlation/python_invoke.cpp
)

set(EXCEPTION_MODULE_DIR
    exception_module/
)

set(EXCEPTION_MODULE_HEADERS
    exception_module/exceptions.hpp
)

set(UTILITY_MODULE_DIR
    utility_module/
)

set(UTILITY_MODULE
    utility_module/date_time.hpp
    utility_module/date_time.cpp
    utility_module/encryption.hpp
    utility_module/encryption.cpp
    utility_module/filesystem_siem.hpp
    utility_module/filesystem_siem.cpp
    utility_module/regex_siem.hpp
    utility_module/regex_siem.cpp
    utility_module/semaphore.hpp
    utility_module/semaphore.cpp
    utility_module/string_manager.hpp
    utility_module/string_manager.cpp
    utility_module/data_storage.hpp
    utility_module/json_proc.hpp
    utility_module/json_proc.cpp
)

set(UTILITY_MODULE_HEADERS
    utility_module/thread_pool.hpp
    utility_module/thread_pool_impl.hpp
)

set(SIEM_STARTUP_MODULE
    siem_startup.hpp
    siem_startup.cpp
    siem_defines.hpp
    siem_defines.cpp
)

include_directories(configs)
include_directories(json_files)
include_directories(samples)
include_directories(source_logs)

include_directories(EXCEPTION_MODULE_DIR)
include_directories(UTILITY_MODULE_DIR)

add_library(aggregation_module ${AGGREGATION_MODULE})
link_libraries(aggregation_module)

add_library(correlation_module ${CORRELATION_MODULE})
link_libraries(correlation_module)

add_library(utility_module ${UTILITY_MODULE})
link_libraries(utility_module)

add_library(siem_startup ${SIEM_STARTUP_MODULE})
link_libraries(siem_startup)

add_executable(${PROJECT_NAME} ${EXCEPTION_MODULE_HEADERS} ${UTILITY_MODULE_HEADERS} "main.cpp")

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})

