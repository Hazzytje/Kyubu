#include "hgl.h"

#include "hassert.h"
#define _CRT_SECURE_NO_WARNINGS /* THANKS, MICROSOFT */
#include <stdio.h>

GLuint hgl::shaders::CompileShader(const char* fileName, int shaderType)
{
	//Handle to be returned 
	GLuint handle;

	//Handle to the file to read from
	FILE* filePointer;
	filePointer = fopen(fileName, "r");

	//TODO: move from assert to throw(?)
	//assert(filePointer != NULL, "Error opening file");

	//Find file size
	fseek(filePointer, 0L, SEEK_END);
    int fileSize = ftell(filePointer);
	rewind(filePointer);

	//Allocate space for file data
	char* fileData = new char[fileSize + 1];
	fread(fileData, 1, fileSize, filePointer);
	fileData[fileSize] = 0;

	//Cast to a const char for the gl function
	const char* fileDataConst = (const char*)fileData;

	//Create new shader, set the source, and compile it
	handle = glCreateShader(shaderType);
	glShaderSource(handle, 1, &fileDataConst, 0);
	glCompileShader(handle);

	//Check if compile is succesfull
	GLint isSuccesfullyCompiled = 0;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &isSuccesfullyCompiled);
	if(isSuccesfullyCompiled == GL_FALSE)
	{
		//Get info log length
		GLint maxLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate data for log
		char* log = new char[maxLength];
		glGetShaderInfoLog(handle, maxLength, &maxLength, log);

		glDeleteShader(handle);

		//TODO: throw error(?)
		printf("Error when compiling shader %s, log:\n%s\n", fileName, log);
		delete[] log;
	}

	delete[] fileData;
	return handle;
}
