#
# Configuration Default
#
ifeq ($(CFG),)
	CFG = Debug
endif

IncludeDir = -I../../../stdext/include -I../../../boost -I../../../boostmm 

#
# Configuration Release
#
ifeq ($(CFG), Release)
	LibDir	    += -LRelease
	Libraries   += -lrt
	Libraries   += -lapr-1
	Libraries   += -lboost-memory
	ProductDir   = Release
	ObjectDir    = Release
	MocFile      = $(ObjectDir)/temp.moc.cpp
	Product      = Release/test
	CFlags       = $(Defines) -fPIC -D__INTEL__ -D__32BIT__ -D__LINUX__ -DLINUX -DNDEBUG -O2 -D_MT -fexceptions 
	CXXFlags     = $(CFlags) -fno-rtti 
	CXX          = g++ -c $(IncludeDir) $(CXXFlags) -o $@
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @$(CXX) $<
	CompileMoc   = @moc $< -o $(MocFile) && $(CXX) $(MocFile) && rm -f $(MocFile)
	Link         = @g++ $(LibDir) -o $(Product)
	CheckLink    = @ldd -u -r $(Product); echo

	LD_LIBRARY_PATH += :$(ProductDir)
endif

#
# Configuration Debug
#
ifeq ($(CFG), Debug)
	LibDir	    += -LDebug
	Libraries   += -lrt
	Libraries   += -lapr-1
	Libraries   += -lboost-memory
	ProductDir   = Debug
	ObjectDir    = Debug
	MocFile      = $(ObjectDir)/temp.moc.cpp
	Product      = Debug/test
	CFlags       = $(Defines) -fPIC -D__INTEL__ -D__32BIT__ -D__LINUX__ -DLINUX -D_DEBUG -g3 -D_MT -fexceptions -Wall 
	CXXFlags     = $(CFlags) -fno-rtti 
	CXX          = g++ -c $(IncludeDir) $(CXXFlags) -o $@
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @$(CXX) $<
	CompileMoc   = @moc $< -o $(MocFile) && $(CXX) $(MocFile) && rm -f $(MocFile)
	Link         = @g++ $(LibDir) -o $(Product)
	CheckLink    = @ldd -u -r $(Product); echo

	LD_LIBRARY_PATH += :$(ProductDir)
endif

#
# Making...
#
all : $(ProductDir) $(ObjectDir) StartMake $(Product) EndMake

$(ProductDir) :
	@mkdir -p $(ProductDir)

ifneq ($(ProductDir),$(ObjectDir))
$(ObjectDir) :
	@mkdir -p $(ObjectDir)
endif

clean :
	@rm -f $(ObjectDir)/*.o
	@rm -f $(Product)

StartMake :
	@echo "-------------- Configuration:" $(PROJ) $(CFG) "--------------"
ifeq ($(ForceLink),1)
	@rm -f $(Product)
endif

EndMake :
	@echo ">>> Success <<<"

#
# Source Files, Objects & Product
#
$(ObjectDir)/RopePerformance.o :	\
	./RopePerformance.cpp
	@echo "---> Compiling" $< "..."
	$(CompileCXX)


Objects =	\
	$(ObjectDir)/RopePerformance.o

$(Product) : $(Objects) $(Libs)
	@echo "---> Creating" $@ "..."
	@rm -f $@
	$(Link) $(Objects) $(Libraries)
	$(CheckLink)

