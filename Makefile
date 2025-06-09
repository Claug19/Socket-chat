CXX = g++
CXXFLAGS = -Wall -Werror -pthread
BINPATH = bin
BUILDPATH = build
TESTINGPATH = Testing
SOURCEPATH = .
LOGPATH = logs

define build
	$(CXX) $(CXXFLAGS) src/$(1).cpp -o $(BINPATH)/$(1)
endef

define run
	./$(BINPATH)/$(1)
endef

# build
buildServer: createBin
	$(call build,chatServer)
buildClient: createBin
	$(call build,chatClient)
cmakeBuild:
	cmake -S $(SOURCEPATH) -B $(BUILDPATH)
	cmake --build $(BUILDPATH)

# run
runServer: buildServer
	$(call run,chatServer)
runClient: buildClient
	$(call run,chatClient)

# run tests
test: cmakeBuild
	cd $(BUILDPATH)&&ctest &&cd ..
cleanTest: clean test

# gdb
gdbServer: buildServer
	gdb $(BINPATH)/chatServer
gdbClient: buildClient
	gdb $(BINPATH)/chatClient

# utility
createBin:
	mkdir -p $(BINPATH)

clean:
	rm -rf $(BINPATH)
	rm -rf $(BUILDPATH)
	rm -rf $(TESTINGPATH)
	rm -rf $(LOGPATH)
	mkdir -p $(LOGPATH)
