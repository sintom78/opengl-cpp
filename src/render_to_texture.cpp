// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
using namespace glm;

#include <common/texture.hpp>
#include <common/controls.hpp>

#include "shader.hpp"
#include "drawableToTexture.hpp"
#include "drawableToScreen.hpp"

static char* sVertexShader = "Simple.vertexshader";
static char* sFragmentShader = "Simple.fragmentshader";

static const float PI = 3.1415926535f;

static const unsigned int sWindowWidth = 1024;
static const unsigned int sWindowHeight = 768;

static GLfloat sZAngle = 0.0f;

static GLfloat sVertices[] = {
   -1.0f, -1.0f, 0,
    1.0f, -1.0f, 0,
    0.0f,  1.0f, 0
};

static GLfloat sColors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f
};


void RenderToScreen(MappedGlUniform4<glm::mat4*>* ModelMatrixID,
                    MappedGlUniform4<glm::mat4*>* VPMatrixID,
                    glm::mat4 *VP,
                    DrawableToScreen *screen);


int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
    window = glfwCreateWindow( sWindowWidth, sWindowHeight, "rendering to texture simple", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPos(window, sWindowWidth/2, sWindowHeight/2);

	// Dark blue background
    //glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
    //glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
    //glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    DrawableToTexture triangle(sWindowWidth, sWindowHeight);
    Program textureProgram;
    textureProgram.attachShader(sVertexShader, GL_VERTEX_SHADER);
    textureProgram.attachShader(sFragmentShader, GL_FRAGMENT_SHADER);
    textureProgram.link();
    triangle.setProgram(&textureProgram);
    GlVbo triangles(sizeof(sVertices),sVertices);
    triangle.AttachVboToAttrib(&triangles,"aPos");
    GlVbo colors(sizeof(sColors),sColors);
    triangle.AttachVboToAttrib(&colors,"aColor");

    //render renderedtexture on:
    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };

    GLuint quad_vertexbuffer;
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

    DrawableToScreen screen;
    Program screenProgram;
    screenProgram.attachShader("Passthrough.vertexshader", GL_VERTEX_SHADER);
    screenProgram.attachShader("WobblyTexture.fragmentshader", GL_FRAGMENT_SHADER);
    screenProgram.link();
    screen.setProgram(&screenProgram);
    MappedGlUniform4<glm::mat4*> ModelMatrixUnif(screenProgram.getGlId(), "ModelM");
    MappedGlUniform4<glm::mat4*> VPMatrixUnif(screenProgram.getGlId(), "VP");
    MappedGlTexture textureunif(screenProgram.getGlId(),"renderedTexture",GL_TEXTURE0,triangle.GetTextureId());
    screen.AttachGlObjToAttrib(&ModelMatrixUnif);
    screen.AttachGlObjToAttrib(&VPMatrixUnif);
    screen.AttachGlObjToAttrib(&textureunif);
    glm::mat4 Projection = glm::perspective(45.0f,
                                            /*float(sWindowWidth / sWindowHeight)*/
                                            4.0f / 3.0f,
                                            0.1f,
                                            100.0f);
    glm::mat4 View       = glm::lookAt(
                                //glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                                //glm::vec3(0,0,0), // and looks at the origin
                                //glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                glm::vec3(0,0,5), // Camera is at (4,3,3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0) //head
                            );

    glm::mat4 VP        = Projection * View;

    GlVbo quadVbo(sizeof(g_quad_vertex_buffer_data),g_quad_vertex_buffer_data);
    MappedGlVbo mapglvbo(&quadVbo,screenProgram.getGlId(),"vertexPosition_modelspace");
    screen.AttachGlObjToAttrib(&mapglvbo);


	do{
        triangle.Draw();

        RenderToScreen(&ModelMatrixUnif,
                       &VPMatrixUnif,
                       &VP,
                       &screen);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

    glDeleteBuffers(1, &quad_vertexbuffer);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void RenderToScreen(MappedGlUniform4<glm::mat4*>* ModelMatrixID,
                    MappedGlUniform4<glm::mat4*>* VPMatrixID,
                    glm::mat4 *VP,
                    DrawableToScreen *screen)
{

    vec3 gPosition1(0.0f, 0.0f, 0.0f);

    sZAngle +=0.01f;
    if (sZAngle>(2*PI)) {
        sZAngle = 0;
    }
    vec3 gOrientation1(0.0f,
                       0.0f,
                       sZAngle);
    vec3 gScaling1(1.0f, 1.0f, 1.0f);

    glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
    glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
    glm::mat4 ScalingMatrix = scale(mat4(), gScaling1);
    glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

    ModelMatrixID->setValue(&ModelMatrix);
    VPMatrixID->setValue(VP);
    screen->Draw();
}
