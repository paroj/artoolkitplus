# ############################################################
#
# Linux project definitions for ARTookKitPlus
#

###
# change target install path

INSTALL_PATH = /usr/local
isEmpty(PREFIX) { PREFIX = $$INSTALL_PATH }
isEmpty(LIBDIR) { LIBDIR = $$PREFIX/lib }

###
# choose between debug and release mode

# CONFIG		= debug
CONFIG		= release

###
# add additional optimization flags (platform-specific)

equals(ARCH, x86_64) {
  # options for AMD64
}
else {
#  QMAKE_CXXFLAGS      = -mtune=pentium4 -march=pentium4 -msse2 -msse -fpermissive
  QMAKE_CXXFLAGS      = -Wall
}


# ############################################################
# DO NOT EDIT BELOW THIS LINE !!!
# ############################################################

VERSION         = 2.0.2

UNAME           = $$system(uname -r)
#message("Found kernel ($$UNAME) ...")

LANGUAGE = C++

debug {
  OBJECTS_DIR     = $$(ARTKP)/build/linux/debug
}

release {
  OBJECTS_DIR     = $$(ARTKP)/build/linux/release
}

DEPENDPATH      += $$(ARTKP)/include

INCLUDEPATH     += $$(ARTKP)/include

LIBS            += -L$$(ARTKP)/lib

# ############################################################
