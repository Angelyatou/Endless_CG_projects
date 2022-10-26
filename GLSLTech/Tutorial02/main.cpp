#include "../CommonLib/common.h"
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
using namespace std;
#include "../CommonLib/Utils.h"
#pragma comment(lib, "CommonLib.lib")
#include "../CommonLib/math3d.h"
#include <assert.h>

const char* pVS = "shader.vert";
const char* pFS = "shader.frag";

GLuint VBO;
GLuint gScaleLocation;

static void RenderSceneCB()
{
		glClear(GL_COLOR_BUFFER_BIT);

		static float Scale = 0.0f;
		Scale += 0.001;
		glUniform1f(gScaleLocation, sinf(Scale));

		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * 4, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(1);

		glutSwapBuffers();
}

static void CreateVertexBuffer()
{
		Vector3f Vertices[3];
		Vertices[0] = Vector3f(-1.0f, -1.0f,0.0f);
		Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
		Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

}

static void AddShader(GLuint ShaderProgram, const char* shaderText, GLenum shaderType)
{
		GLuint ShaderObj = glCreateShader(shaderType);
		if (ShaderObj == 0)
		{
				ErrorOut();
				exit(1);
		}

		const GLchar* p[1];
		p[0] = shaderText;
		GLint L[1];
		L[0] = strlen(shaderText);

		glShaderSource(ShaderObj, 1, p, L);

		glCompileShader(ShaderObj);

		GLint success;
		glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
		if (!success) {
				GLchar InfoLog[1024];
				glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
				fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
				exit(1);
		}

		glAttachShader(ShaderProgram, ShaderObj);
}

void CompileShaders()
{
		string vs, fs;
		if (!ReadFile(pVS, vs))
		{
				exit(1);
		}
		if (!ReadFile(pFS, fs))
		{
				exit(1);
		}

// 		cout << vs << endl;
// 		cout << fs << endl; 

		GLuint ShaderProgram = glCreateProgram();
		if (0 == ShaderProgram)
		{
				ErrorOut();
				exit(1);
		}

		AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
		AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

		glLinkProgram(ShaderProgram);
		GLint Success = 0;
		GLchar ErrorLog[1024] = { 0 };
		glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
		if (Success == 0) {
				glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
				fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
				exit(1);
		}

		gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
		assert(gScaleLocation != 0xFFFFFFFF);

		glValidateProgram(ShaderProgram);
		glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
		if (Success == 0) {
				glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
				fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
				exit(1);
		}

		glUseProgram(ShaderProgram);
}

static void InitializeGlutCallbacks()
{
		glutDisplayFunc(RenderSceneCB);
		glutIdleFunc(RenderSceneCB);
}

int main(int argc, char** argv)
{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
		glutInitWindowSize(800, 600);
		glutInitWindowPosition(100, 100);
		glutCreateWindow("Tutorial 02");

		InitializeGlutCallbacks();

		// Must be done after glut is initialized!
		GLenum res = glewInit();
		if (res != GLEW_OK) {
				fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
				return 1;
		}

		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

		CompileShaders();

		CreateVertexBuffer();

		glutMainLoop();

		return 0;
}