##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=PSO_user
ConfigurationName      :=Debug
WorkspacePath          :="C:/Users/games/Google Drive/Master Brasil/CN_2016_DIBS"
ProjectPath            :="C:/Users/games/Google Drive/Master Brasil/CN_2016_DIBS/PSO_user"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=games
Date                   :=09/03/2017
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/MinGW/bin/g++.exe
SharedObjectLinkerName :=C:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="PSO_user.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  -static-libgcc -static-libstdc++
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall -fpermissive $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -fpermissive $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/kiss_fft.c$(ObjectSuffix) $(IntermediateDirectory)/kiss_fftr.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/games/Google Drive/Master Brasil/CN_2016_DIBS/PSO_user/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/kiss_fft.c$(ObjectSuffix): kiss_fft.c $(IntermediateDirectory)/kiss_fft.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/games/Google Drive/Master Brasil/CN_2016_DIBS/PSO_user/kiss_fft.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/kiss_fft.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/kiss_fft.c$(DependSuffix): kiss_fft.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/kiss_fft.c$(ObjectSuffix) -MF$(IntermediateDirectory)/kiss_fft.c$(DependSuffix) -MM kiss_fft.c

$(IntermediateDirectory)/kiss_fft.c$(PreprocessSuffix): kiss_fft.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/kiss_fft.c$(PreprocessSuffix) kiss_fft.c

$(IntermediateDirectory)/kiss_fftr.c$(ObjectSuffix): kiss_fftr.c $(IntermediateDirectory)/kiss_fftr.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/games/Google Drive/Master Brasil/CN_2016_DIBS/PSO_user/kiss_fftr.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/kiss_fftr.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/kiss_fftr.c$(DependSuffix): kiss_fftr.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/kiss_fftr.c$(ObjectSuffix) -MF$(IntermediateDirectory)/kiss_fftr.c$(DependSuffix) -MM kiss_fftr.c

$(IntermediateDirectory)/kiss_fftr.c$(PreprocessSuffix): kiss_fftr.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/kiss_fftr.c$(PreprocessSuffix) kiss_fftr.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


