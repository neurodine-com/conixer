# a Conixer program - a GAME client communicating in STD-inn/out

# OBJECTIVE:
    I want this Comixer project to compile on Linux into @./bin directory into single executable.
    Create any files neceserry make this happen.
    Modify any sources in @./src location to complete the OBJECTIVE

    In case of possible issues You will struct, like:
    1.) fix `#include` errors in source files
    2.) fix syntax errors or indentation errors or any other errors blocking ability for the files to compile into a executable program
    3.) Create a makefile file inside main directory so that it allows to run compile/build of the game client on linux
    
    Fix Them by modyfying files to fullfil the objective.   

    Changes made to files, new files added to the folder, present in a form of report inside README-WHATDONE.md (create if missing).
    Changes to @./src/*.*.h files present as unified diff in form of a patch file.

    Save Your work into SESSIOn-OMP.md in a form that allows any other agent to start on work from place where You stopped.

#  Most important:
    Create a linux makefile from the CodeBlocks IDE project, layout, depends files presented below. The project MUST compile with C++/G++ from commandline on linux by using "make".
    If any tools from wich the `make` tool depends are missing, inform me and I will install them.

# C/C++ source files location
    @./src/comixer.cbp - is a CodeBlocks IDE file a project "CodeBlocks_project_file" - use it to create a makefile:
```XML
<?xml version="1.0"?>
<!DOCTYPE CodeBlocks_project_file>
<CodeBlocks_project_file>
	<FileVersion major="1" minor="1"/>
	<Project>
		<Option title="Console application"/>
		<Option makefile="Makefile"/>
		<Option makefile_is_custom="0"/>
		<Option compiler="0"/>
		<Build>
			<Target title="default">
				<Option output="bin/comixer.exe"/>
				<Option working_dir="."/>
				<Option object_output=".objs"/>
				<Option deps_output=".deps"/>
				<Option type="1"/>
				<Option compiler="0"/>
				<Option projectResourceIncludeDirsRelation="1"/>
				<Compiler>
					<Add option="-g"/>
				</Compiler>
			</Target>
		</Build>
		<Compiler>
			<Add option="-g"/>
		</Compiler>
		<Unit filename="FieldSpace.h">
			<Option compilerVar="CPP"/>
			<Option compile="0"/>
			<Option link="0"/>
			<Option target="default"/>
		</Unit>
		<Unit filename="main.cpp">
			<Option compilerVar="CPP"/>
			<Option target="default"/>
		</Unit>
		<Unit filename="path_finder.h">
			<Option compilerVar="CPP"/>
			<Option compile="0"/>
			<Option link="0"/>
			<Option target="default"/>
			<Breakpoint line="136" enabled="1" pass="0" />
		</Unit>
		<Unit filename="tel.h">
			<Option compilerVar="CPP"/>
			<Option compile="0"/>
			<Option link="0"/>
			<Option target="default"/>
		</Unit>
	</Project>
</CodeBlocks_project_file>
```
    @./src/comixer.depend - depslib dependency file v1.0
```
# depslib dependency file v1.0
1161995986 src/main.cpp
	<cstdlib>
	<iostream>
	<fstream>
	<sstream>
	<src/tel.h>
	<src/FieldSpace.h>
	<src/path_finder.h>

1161527824 src/main.cpp
	<cstdlib>
	<iostream>
	<fstream>
	<src/tel.h>
	<src/FieldSpace.h>
	<src/path_finder.h>
```
    @./src/comixer.layout  - is a layout file: "CodeBlocks_layout_file"
```XML
<?xml version="1.0"?>
<!DOCTYPE CodeBlocks_layout_file>
<CodeBlocks_layout_file>
	<File name="FieldSpace.h" open="1" top="0">
		<Cursor position="11726" topLine="0"/>
	</File>
	<File name="main.cpp" open="0" top="0">
		<Cursor position="2309" topLine="0"/>
	</File>
	<File name="path_finder.h" open="1" top="1">
		<Cursor position="138" topLine="0"/>
	</File>
	<File name="tel.h" open="1" top="0">
		<Cursor position="199" topLine="0"/>
	</File>
</CodeBlocks_layout_file>
```


    
