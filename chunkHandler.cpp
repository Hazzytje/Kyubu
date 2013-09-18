#include <stdio.h>

#include "matrix.h"
#include "globals.h"
#include "hgl.h"
#include "chunkHandler.h"
#include "textureloader.h"

ChunkHandler::ChunkHandler()
{
	//shaders and shaderprogram
	GLuint vertexShader = hgl::shaders::CompileShader("vertShader.txt", GL_VERTEX_SHADER);
	GLuint fragmentShader = hgl::shaders::CompileShader("fragShader.txt", GL_FRAGMENT_SHADER);

	shaderProgramHandle = glCreateProgram();
	glAttachShader(shaderProgramHandle, vertexShader);
	glAttachShader(shaderProgramHandle, fragmentShader);
	glBindFragDataLocation(shaderProgramHandle, 0, "outColor");

	glLinkProgram(shaderProgramHandle);
	glUseProgram(shaderProgramHandle);

	//texture
	glGenTextures(1, &textureHandle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	auto image = Loadedimage("terrain.bmp");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixelData);

	glUniform1i(glGetUniformLocation(shaderProgramHandle, "tex"), 0);

	viewMatrixLocation = glGetUniformLocation(shaderProgramHandle, "view");
	modelMatrixLocation = glGetUniformLocation(shaderProgramHandle, "model");

	GLint projMatrixLocation = glGetUniformLocation(shaderProgramHandle, "proj");
	Matrix proj = Matrix::CreatePerspective(float(3.1415f * 0.5f), 1024.0f / 768.0f, 0.1f, 1000.0f);

	const float* projMatrix = proj.ToFloatArray();

	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, projMatrix);
	ChunkList = std::vector<Chunk*>();
	for(int x = 0; x < 4; x++)
	{
		for(int y = 0; y < 4; y++)
		{
			printf("chunk x: %i y: %i \n", x, y);
			Chunk* testChunk = new Chunk(shaderProgramHandle);
			testChunk->chunkX = x;
			testChunk->chunkY = y;

			ChunkList.push_back(testChunk);
		}
	}

	glUseProgram(0);
}

ChunkHandler::~ChunkHandler()
{

}

void ChunkHandler::Render()
{
		Globals::PrintAllGlErrors();
		printf("at file %s, line %i\n", __FILE__, __LINE__);
	glUseProgram(shaderProgramHandle);
		Globals::PrintAllGlErrors();
		printf("at file %s, line %i\n", __FILE__, __LINE__);
	glActiveTexture(GL_TEXTURE0);
		Globals::PrintAllGlErrors();
		printf("at file %s, line %i\n", __FILE__, __LINE__);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
		Globals::PrintAllGlErrors();
		printf("at file %s, line %i\n", __FILE__, __LINE__);
	Globals::camera.InsertViewMatrix(viewMatrixLocation);
		Globals::PrintAllGlErrors();
		printf("at file %s, line %i\n", __FILE__, __LINE__);
	for(auto chunk : ChunkList)
	{
		Matrix modelMatrix = Matrix::CreateTranslation(chunk->chunkX * 16, chunk->chunkY * 16, 0);
//		modelMatrix.DebugPrint();

		glUniformMatrix4fv(modelMatrixLocation, 1, GL_TRUE, modelMatrix.ToFloatArray());
		chunk->Render();
	}
		Globals::PrintAllGlErrors();
		printf("at file %s, line %i\n", __FILE__, __LINE__);
	glBindVertexArray(0);
	glUseProgram(0);
}
