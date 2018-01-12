if [ $# -ne 1 ]; then
    printf "\n\tThe library name must be passed as a paramater. \n\n"
    exit 1
fi

library=$1
if [ ! -d $library ]; then  
	mkdir $library
fi

if [ ! -d "include" ]; then  
	mkdir -p include/$library
else
	cd include
	if [ ! -d $library ]; then  
		mkdir $library
	fi
	cd ..
fi

cd $library

if [ ! -d "source" ]; then  
	mkdir "source"
fi

if [ ! -f "CMakeLists.txt" ]; then  
	touch CMakeLists.txt
	printf "project ("$library") \n\n" >> CMakeLists.txt
	printf "set (Dependencies ) \n\n" >> CMakeLists.txt
	printf "include_directories (\${CMAKE_SOURCE_DIR}/include) \n\n" >> CMakeLists.txt
	printf "file (GLOB_RECURSE \${PROJECT_NAME}_sources \${CMAKE_SOURCE_DIR}/\${PROJECT_NAME}/source/*.cpp) \n\n" >> CMakeLists.txt
	printf "add_library (\${PROJECT_NAME} \${\${PROJECT_NAME}_sources}) \n\n" >> CMakeLists.txt
	printf "foreach (Dependency \${Dependencies}) \n" >> CMakeLists.txt
	printf "\ttarget_link_libraries (\${PROJECT_NAME} \${Dependency}) \n" >> CMakeLists.txt
	printf "endforeach ()" >> CMakeLists.txt
fi