# Compiler and tools
CXX := g++
RM := rm -f
MKDIR_P := mkdir -p

# Project
TARGET := turn_based_game

# Source and build directories
SRCDIR := .
OBJDIR := obj
BINDIR := bin

# Files
SRCS := main.cpp GameManager.cpp MapManager.cpp Player.cpp Enemy.cpp Character.cpp Dungeon.cpp Boss.cpp BattleSystem.cpp
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# SFML Libraries
SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system

# Build types
DEBUG_CXXFLAGS := -Wall -g -std=c++11
RELEASE_CXXFLAGS := -Wall -O2 -std=c++11

CXXFLAGS := $(DEBUG_CXXFLAGS)

# Default target
all: debug

# Debug build
debug: CXXFLAGS := $(DEBUG_CXXFLAGS)
debug: build_dirs $(BINDIR)/$(TARGET)

# Release build
release: CXXFLAGS := $(RELEASE_CXXFLAGS)
release: build_dirs $(BINDIR)/$(TARGET)

# Build directories
build_dirs:
	$(MKDIR_P) $(OBJDIR) $(BINDIR)

# Link
$(BINDIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(SFML_LIBS)

# Compile source files
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependencies
-include $(DEPS)

# Clean tartgets
clean:
	$(RM) -r $(OBJDIR) $(BINDIR)
	$(RM) *.o

# Remove dependencies
clean_deps:
	$(RM) $(DEPS)

.PHONY: all debug release clean clean_deps build_dirs


