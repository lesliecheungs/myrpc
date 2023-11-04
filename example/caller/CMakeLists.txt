set(SRC_LIST calluserservice.cc ../entrywebserver.pb.cc)
add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer mprpc protobuf)