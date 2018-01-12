if [ $# -ne 1 ]; then
    printf "\n\tThe library name must be passed as a paramater. \n\n"
    exit 1
fi

test="$1"Test""
if [ ! -d $test ]; then  
	mkdir $test
fi

cd $test
if [ ! -d "source" ]; then  
	mkdir "source"
fi

if [ ! -f "CMakeLists.txt" ]; then  
	touch CMakeLists.txt
	printf "project ("$test") \n\n" >> CMakeLists.txt
	printf "set (Dependencies "$library") \n\n" >> CMakeLists.txt
	printf "include_directories (\${CMAKE_SOURCE_DIR}/include) \n\n" >> CMakeLists.txt
	printf "file (GLOB_RECURSE \${PROJECT_NAME}_sources \${CMAKE_SOURCE_DIR}/\${PROJECT_NAME}/source/*.cpp) \n\n" >> CMakeLists.txt
	printf "add_executable (\${PROJECT_NAME} \${\${PROJECT_NAME}_sources}) \n\n" >> CMakeLists.txt
	printf "foreach (Dependency \${Dependencies}) \n" >> CMakeLists.txt
	printf "\ttarget_link_libraries (\${PROJECT_NAME} \${Dependency}) \n" >> CMakeLists.txt
	printf "endforeach ()" >> CMakeLists.txt
fi