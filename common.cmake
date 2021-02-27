# make common .h files visible
include_directories(../lib)

# c is the only language one ever needs
include(../language.cmake)

# turn all reasonable gcc warnings
include(../warnings.cmake)
