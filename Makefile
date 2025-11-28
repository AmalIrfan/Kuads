PLATFORM?=linux
TARGET?=linux

ifeq ($(PLATFORM),linux)
	ifeq ($(TARGET),linux)
		CXX=g++
	else ifeq ($(TARGET),windows)
		CXX=x86_64-w64-mingw32-g++
		EXTRA_CXXFLAGS=-static
		EXTRA_LDFLAGS=-lgdi32 -lwinmm
	else
		$(error Invalid target)
	endif
else ifeq ($(PLATFORM),windows)
	ifeq ($(TARGET),windows)
		CXX=g++
		EXTRA_CXXFLAGS=-static
		EXTRA_LDFLAGS=-lgdi32 -lwinmm
	else
		$(error Invalid target)
	endif
else
	$(error Invalid platform)
endif

ifdef RELEASE
	EXTRA_CXXFLAGS+=-Os
endif

define make_build_dir
	@[ -d $(dir $@) ] || mkdir -p $(dir $@) && echo "*" > $(dir $@)/.gitignore
endef

RAYLIB_VERSION=5.5
RAYLIB=vendor/${TARGET}/raylib-${RAYLIB_VERSION}/

CXXFLAGS=-I${RAYLIB}/include ${EXTRA_CXXFLAGS}
LDFLAGS=-L${RAYLIB}/lib -l:libraylib.a -lm ${EXTRA_LDFLAGS}

EXEC=build/${TARGET}/main${EXEC_EXT}

${EXEC}: main.cpp
	$(call make_build_dir)
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}
	@echo "Build complete: $@"

.PHONY: clean

clean:
	rm -rf build
