add_library(ParabolicPathSmooth STATIC DynamicPath.cpp paraboliccommon.cpp pramp.cpp pramp.h ppramp.cpp ppramp.h plpramp.cpp plpramp.h ParabolicRamp.cpp Timer.cpp DynamicPath.h ParabolicRamp.h paraboliccommon.h Timer.h)
set_target_properties(ParabolicPathSmooth PROPERTIES COMPILE_FLAGS "${PLUGIN_COMPILE_FLAGS}" LINK_FLAGS "${PLUGIN_LINK_FLAGS}")
# add_dependencies(ParabolicPathSmooth interfacehashes_target)

target_link_libraries(ParabolicPathSmooth PUBLIC openrave) # PRIVATE boost_assertion_failed)

add_executable(testparabolicramp testparabolicramp.cpp)
# should use boost_system instead of ${Boost_SYSTEM_LIBRARY}
target_link_libraries(testparabolicramp ParabolicPathSmooth openrave ${LOG4CXX_LIBRARY} boost_system)
