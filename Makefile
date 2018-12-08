FLUXIONS_SRCDIR = src
FLUXIONS_INCDIR = include
FLUXIONS_OBJDIR = build
FLUXIONS_SOURCES = $(wildcard $(FLUXIONS_SRCDIR)/*.cpp)
FLUXIONS_HEADERS = $(wildcard $(FLUXIONS_INCDIR)/*.hpp)
FLUXIONS_OBJECTS = $(patsubst $(FLUXIONS_SRCDIR)/%.cpp,$(FLUXIONS_OBJDIR)/%.o,$(FLUXIONS_SOURCES))
FLUXIONS_TARGET = build/libfluxions.a
FLUXIONS_GCH = $(FLUXIONS_SRCDIR)/stdafx.h.gch
# requires the following packages
# libczmq-dev libzmq3-dev libcurl4-gnutls-dev libsodium zlib1g-dev python3-dev mesa-dev

DEP_INCDIR = dep/include
DEP_SRCDIR = dep/src
DEP_SOURCES = $(wildcard $(DEP_SRCDIR)/*.c*)
DEP_HEADERS = $(wildcard $(DEP_INCDIR)/*.h*)
DEP_OBJECTS = $(DEP_SOURCES:.cpp=.o)

SSPHH_DIR = ssphh
SSPHH_SOURCES = $(wildcard ssphh/src/*.cpp)
SSPHH_HEADERS = $(wildcard ssphh/include/*.hpp)
SSPHH_OBJECTS = $(SSPHH_SOURCES:.cpp=.o)

SRCS = src/
CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -I$(FLUXIONS_INCDIR) -I$(DEP_INCDIR) `python3-config --includes`
LDFLAGS = -LGLEW -LGL -LGLU -Lglut

.PHONY: all clean precompiled

all: $(FLUXIONS_TARGET)
# ssphh libfluxions.a

precompiled: $(FLUXIONS_GCH)

$(FLUXIONS_TARGET): $(FLUXIONS_OBJECTS) $(DEP_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(FLUXIONS_OBJECTS) $(DEP_OBJECTS)

$(FLUXIONS_GCH): $(FLUXIONS_SRCDIR)/stdafx.h $(FLUXIONS_HEADERS) $(DEP_HEADERS)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(FLUXIONS_OBJDIR)/%.o: $(FLUXIONS_SRCDIR)/%.cpp $(FLUXIONS_GCH)
	$(CXX) -c $(CXXFLAGS) $< -o $@

GTAGS: $(FLUXIONS_SOURCES) $(SSPHH_SOURCES)
	gtags

clean:
	rm $(FLUXIONS_GCH)
	rm $(FLUXIONS_OBJDIR)/*.o
	rm $(FLUXIONS_TARGET)
	rm GPATH GRTAGS GTAGS
