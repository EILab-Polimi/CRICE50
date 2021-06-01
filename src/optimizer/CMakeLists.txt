add_library(opt Optimizer.cpp Optimizer.h)
target_link_libraries(opt "${TORCH_LIBRARIES}")
set_property(TARGET opt PROPERTY CXX_STANDARD 14)