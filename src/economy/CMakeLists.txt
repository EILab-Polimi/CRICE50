add_library(economies Econ.cpp Econ.h EconAgent.cpp EconAgent.h)
target_link_libraries(economies "${TORCH_LIBRARIES}")
#set_property(TARGET economies PROPERTY CXX_STANDARD 14)