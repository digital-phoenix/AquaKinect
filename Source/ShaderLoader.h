#ifndef __SHADER_LOADER__
#define __SHADER_LOADER__

#include <windows.h>
#include<gl/glew.h>

#include<string>
#include <iostream>
#include<string>

class ShaderLoader{

	public:
		static GLuint createShader( std::string vectorShader, std::string fragmentShader);
	private:
		static unsigned long getFileSize(std::istream &file);
		static int loadShader( std::string file, std::string &shaderSource, unsigned long *length);
		inline static void printLog(std::string s){ std::cout<< __FILE__ <<" " << __LINE__ << " "<< s;}
		static int compileShader(GLuint shaderObject);
};

#endif