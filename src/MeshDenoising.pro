TEMPLATE = subdirs

QMAKE_CXXFLAGS += -O3 -ffast-math
QMAKE_CXXFLAGS_RELEASE += -O3 -ffast-math

SUBDIRS += \
    OpenMesh \
    Denoising

Denoising.depends = OpenMesh
