#  Process this file with automake to produce Makefile.in
include $(top_srcdir)/Makefile.common

AM_CPPFLAGS += -I @top_srcdir@/contrib/spark/include

noinst_LIBRARIES = libsparkcore.a
libsparkcore_a_SOURCES = \
	SPK_Connections.cpp \
	SPK_DataSet.cpp \
	SPK_DEF.cpp \
	SPK_Emitter.cpp \
	SPK_Factory.cpp \
	SPK_Group.cpp \
	SPK_Logger.cpp \
	SPK_MemoryTracer.cpp \
	SPK_Object.cpp \
	SPK_Octree.cpp \
	SPK_Renderer.cpp \
	SPK_System.cpp \
	SPK_Transformable.cpp \
	SPK_Transform.cpp \
	SPK_Types.cpp \
	SPK_Zone.cpp \
	SPK_ZonedModifier.cpp
