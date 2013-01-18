/*
 * GLUtils.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: Guillaume Chatelet
 */

#include "GLUtils.h"

#include <duke/gl/GL.h>
#include <stdexcept>

void glCheckError() {
	switch (glGetError()) {
	case GL_INVALID_ENUM:
		throw std::runtime_error("OpenGL : Invalid enum");
	case GL_INVALID_VALUE:
		throw std::runtime_error("OpenGL : Invalid value");
	case GL_INVALID_OPERATION:
		throw std::runtime_error("OpenGL : Invalid operation");
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		throw std::runtime_error("OpenGL : Invalid framebuffer operation");
	case GL_OUT_OF_MEMORY:
		throw std::runtime_error("OpenGL : Out of memory");
	case GL_STACK_UNDERFLOW:
		throw std::runtime_error("OpenGL : Stack underflow");
	case GL_STACK_OVERFLOW:
		throw std::runtime_error("OpenGL : Stack overflow");
	}
}

static GLuint getBindParameter(GLuint targetType) {
	switch (targetType) {
	case GL_TEXTURE_2D:
		return GL_TEXTURE_BINDING_2D;
	case GL_TEXTURE_RECTANGLE:
		return GL_TEXTURE_BINDING_RECTANGLE;
	case GL_ARRAY_BUFFER:
		return GL_ARRAY_BUFFER_BINDING;
	case GL_ELEMENT_ARRAY_BUFFER:
		return GL_ELEMENT_ARRAY_BUFFER_BINDING;
	case GL_PIXEL_UNPACK_BUFFER:
		return GL_PIXEL_UNPACK_BUFFER_BINDING;
	case GL_PIXEL_PACK_BUFFER:
		return GL_PIXEL_PACK_BUFFER_BINDING;
	};
	throw std::runtime_error("unsupported targetType");
}

void glCheckBound(GLuint targetType, GLuint id) {
	GLint current;
	glGetIntegerv(getBindParameter(targetType), &current);
	if (GLuint(current) != id)
		throw std::runtime_error("Trying to operate on unbound GlObject");
}

void checkShaderError(GLuint shaderId, const char* pSource) {
	GLint success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success)
		return;
	GLchar InfoLog[1024];
	glGetShaderInfoLog(shaderId, sizeof(InfoLog), NULL, InfoLog);
	char error[1024];
	snprintf(error, sizeof(error), "OpenGL : error compiling shader type :\n%s\nsource :'%s'", InfoLog, pSource);
	throw std::runtime_error(error);
}

void checkProgramError(unsigned int programId) {
	GLint success;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (success)
		return;
	GLchar InfoLog[1024];
	glGetProgramInfoLog(programId, sizeof(InfoLog), NULL, InfoLog);
	char error[1024];
	snprintf(error, sizeof(error), "OpenGL : linking shader program :\n%s\n", InfoLog);
	throw std::runtime_error(error);
}

#include <fstream>
#include <sstream>

std::string slurpFile(const char* pFilename) {
	std::ifstream in(pFilename);
	if (!in)
		throw std::ios_base::failure(std::string("unable to load file : ") + pFilename);
	std::stringstream sstr;
	sstr << in.rdbuf();
	return sstr.str();
}
