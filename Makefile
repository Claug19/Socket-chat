#### PROJECT CONFIG ####
PROJECTNAME = Socket-chat
SOURCEPATH = .
BINPATH = bin
BUILDPATH = build
CONFIGPATH = config
INCLUDEPATH = include
LOGPATH = logs
UTILSPATH = src/utils


#### COMPILER CONFIG ####
CXX = g++
CXXFLAGS = -I$(INCLUDEPATH) -Wall -Werror -pthread


#### FILES ####
ADDITIONALFILES = \
	$(UTILSPATH)/getConfig.cpp \
	$(UTILSPATH)/logger.cpp


#### MAKEFILE FUNCTIONS ####
define quickBuild
	$(CXX) $(CXXFLAGS) src/$(1).cpp $(ADDITIONALFILES) -o $(BINPATH)/$(1)
	cp -r $(CONFIGPATH) $(BINPATH)
endef

define cmakeBuild
	cmake -S $(SOURCEPATH) -B $(1) -DCMAKE_BUILD_TYPE=$(2)
	cmake --build $(1)
endef

define run
	cd $(BINPATH)&&./$(1)&&cd ..
endef


#### QUICK BUILDS (gcc + bin) ####
qBuildServer: createBin
	$(call quickBuild,chatServer)
qBuildClient: createBin
	$(call quickBuild,chatClient)


#### GDB BUILDS (gcc + bin + gdb) ####
gdbServer: buildServer
	gdb $(BINPATH)/chatServer
gdbClient: buildClient
	gdb $(BINPATH)/chatClient


#### CMAKE BUILDS (cmake + build) ####
## DEFAULT ##
build:
	$(eval BUILDSUBPATH := $(BUILDPATH)/$(PROJECTNAME))
	$(call cmakeBuild,$(BUILDSUBPATH))

## DEBUG ##
debugBuild:
	$(eval BUILDSUBPATH := $(BUILDPATH)/debug)
	$(eval CMAKE_BUILD_TYPE := Debug)
	$(call cmakeBuild,$(BUILDSUBPATH),$(CMAKE_BUILD_TYPE))

## RELEASE ##
releaseBuild:
	$(eval BUILDSUBPATH := $(BUILDPATH)/release)
	$(eval CMAKE_BUILD_TYPE := Release)
	$(call cmakeBuild,$(BUILDSUBPATH),$(CMAKE_BUILD_TYPE))

## RELEASE WITH DEBUG INFO ##
rwdiBuild:
	$(eval BUILDSUBPATH := $(BUILDPATH)/rwdi)
	$(eval CMAKE_BUILD_TYPE := RelWithDebInfo)
	$(call cmakeBuild,$(BUILDSUBPATH),$(CMAKE_BUILD_TYPE))

## MINIMAL SIZE RELEASE ##
msrBuild:
	$(eval BUILDSUBPATH := $(BUILDPATH)/msr)
	$(eval CMAKE_BUILD_TYPE := MinSizeRel)
	$(call cmakeBuild,$(BUILDSUBPATH),$(CMAKE_BUILD_TYPE))

## UNIT TESTS ##
testBuild:
	$(eval BUILDSUBPATH := $(BUILDPATH)/$(PROJECTNAME))
	$(eval CMAKE_BUILD_TYPE := Test)
	$(call cmakeBuild,$(BUILDSUBPATH),$(CMAKE_BUILD_TYPE))

## UNIT TESTS & COVERAGE ##
coverageBuild:
	$(eval BUILDSUBPATH := $(BUILDPATH)/$(PROJECTNAME))
	$(eval CMAKE_BUILD_TYPE := Coverage)
	$(call cmakeBuild,$(BUILDSUBPATH),$(CMAKE_BUILD_TYPE))


#### RUN BINARIES ####
runServer: qBuildServer
	$(call run,chatServer)
runClient: qBuildClient
	$(call run,chatClient)


#### RUN UNIT TESTS ####
test: testBuild
	cd $(BUILDPATH)/$(PROJECTNAME)/test&&ctest &&cd ../..
cleanTest: clean test


#### UTILITY FUNCTIONS ####
createBin:
	mkdir -p $(BINPATH)

clean:
	rm -rf $(BINPATH)
	rm -rf $(BUILDPATH)
