###################################################################################
#                                                                                 #
# NAME: lib/meson.build                                                           #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################



c_exception_src = files('CException.c')

c_exception_dir = include_directories('.')

c_exception_lib = library(meson.project_name(), 
    sources: c_exception_src,
    include_directories: c_exception_dir)