//
//  planet.cpp
//  COSC3000
//
//  Created by Keyne Kassapa on 26/5/19.
//  Copyright © 2019 Keyne Kassapa. All rights reserved.
//

#include "planet.hpp"
#include <iostream>

// Lib includes
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../SOIL2/SOIL2.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// local includes
#include "shader.h"
#include "camera.h"
#include "model.h"

const GLint WIDTH = 1400, HEIGHT = 800;
const double PI = 3.141592653589793238463;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera
Camera camera( glm::vec3( 40.0f, 0.0f, 30.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
string cameraType = "";

// Light attributes
glm::vec3 lightPos( 0.0f, 0.0f, 0.0f );

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat speed = 1.0f;

int planet() {
    
    bool move = true;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "learn Opengl", nullptr, nullptr);
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    glewExperimental = GL_TRUE;
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    glEnable( GL_DEPTH_TEST );
    
    Shader shader( "resources/shaders/modelLoading.vs", "resources/shaders/modelLoading.frag" );
    Shader directionalShader( "resources/shaders/directional.vs", "resources/shaders/directional.frag" );
    Shader lampShader( "resources/shaders/lamp.vs", "resources/shaders/lamp.frag" );
    
    // Load models
    Model earthModel( "resources/models/earth/Earth.obj" );
    Model space( "resources/models/space/space.obj" );
    Model sunModel( "resources/models/sun/sun.obj" );
    Model mercuryModel( "resources/models/mercury/mercury.obj" );
    Model venusModel( "resources/models/venus/venus.obj" );
    Model marsModel( "resources/models/mars/mars.obj" );
    Model jupiterModel("resources/models/jupiter/jupiter.obj");
    Model saturnModel("resources/models/saturn/13906_Saturn_v1_l3.obj");
    Model uranusModel("resources/models/uranus/13907_Uranus_v2_l3.obj");
    Model neptuneModel("resources/models/neptune/13908_Neptune_v2_l3.obj");
    
//    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glm::mat4 projection(1);
    projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    GLfloat scale = 0.1f;
    GLuint i = 0;
    while( !glfwWindowShouldClose( window ) )
    {
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        i ++;
        glfwPollEvents( );
        DoMovement( );
        glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        
        glm::mat4 view(1);
        view = camera.GetViewMatrix( );
        
        directionalShader.Use( );
        
        // Set lights properties
        glUniform3f( glGetUniformLocation( directionalShader.Program, "light.position" ), lightPos.x, lightPos.y, lightPos.z );
        glUniform3f( glGetUniformLocation( directionalShader.Program, "light.ambient" ),  0.2f, 0.2f, 0.2f );
        glUniform3f( glGetUniformLocation( directionalShader.Program, "light.diffuse" ),  1.5f, 1.5f, 1.5f );
        glUniform3f( glGetUniformLocation( directionalShader.Program, "light.specular" ), 0.0f, 0.0f, 0.0f);
        
        glUniformMatrix4fv( glGetUniformLocation( directionalShader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( directionalShader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        
        // Model
        
        // SPACE
        glm::mat4 model(1);
        model = glm::translate( model, glm::vec3( 0.0f, 0.0f, 0.0f) );
        model = glm::scale( model, glm::vec3( 20.0f ) );
        glUniformMatrix4fv( glGetUniformLocation( directionalShader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        space.Draw( directionalShader );
        
        GLfloat angle, radius, x, y;
        
        shader.Use( );
        GLint viewPosLoc  = glGetUniformLocation( shader.Program, "viewPos" );
        glUniform3f( viewPosLoc,  camera.GetPosition( ).x, camera.GetPosition( ).y, camera.GetPosition( ).z );
        
        // Set lights properties
        glUniform3f( glGetUniformLocation( shader.Program, "light.position" ), lightPos.x, lightPos.y, lightPos.z );
        glUniform3f( glGetUniformLocation( shader.Program, "light.ambient" ),  0.2f, 0.2f, 0.2f );
        glUniform3f( glGetUniformLocation( shader.Program, "light.diffuse" ),  1.5f, 1.5f, 1.5f );
        glUniform3f( glGetUniformLocation( shader.Program, "light.specular" ), 0.0f, 0.0f, 0.0f);
        glUniform1f( glGetUniformLocation( shader.Program, "light.constant" ), 1.0f );
        glUniform1f( glGetUniformLocation( shader.Program, "light.linear" ), 0.02f );
        glUniform1f( glGetUniformLocation( shader.Program, "light.quadratic" ), 0.006f );
        
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        
        // MERCURY
        model = glm::mat4(1);
        if (move) {
            angle = 0.008f * i * speed;
            radius = 70.0f * scale;
            x = radius * sin(PI * 2 * angle / 360);
            y = radius * cos(PI * 2 * angle / 360);
            model = glm::translate( model, glm::vec3( x, 0.0f, y) );
        } else {
            model = glm::translate( model, glm::vec3( 70.0f * scale, 0.0f, 0.0f) );
        }
        
        if (cameraType == "Mercury") {
            camera.SetPosition(glm::vec3(x + 0.5f, 0.0f, y+0.5f));
        }
        
        model = glm::scale( model, glm::vec3( 0.3f * scale ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        mercuryModel.Draw( shader );
        
        // VENUS
        model = glm::mat4(1);
        if (move) {
            angle = 0.007f * i  * speed;
            radius = 80.0f * scale;
            x = radius * sin(PI * 2 * angle / 360);
            y = radius * cos(PI * 2 * angle / 360);
            model = glm::translate( model, glm::vec3( x, 0.0f, y) );
        } else {
            model = glm::translate( model, glm::vec3( 80.0f * scale, 0.0f, 0.0f) );
        }
        
        if (cameraType == "Venus") {
            camera.SetPosition(glm::vec3(x + 0.5f, 0.0f, y+0.5f));
        }
        
        model = glm::scale( model, glm::vec3( 0.5f * scale ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        venusModel.Draw( shader );
        
        // EARTH
        model = glm::mat4(1);
        if (move) {
            angle = 0.006f * i  * speed;
            radius = 90.0f * scale;
            x = radius * sin(PI * 2 * angle / 360);
            y = radius * cos(PI * 2 * angle / 360);
            model = glm::translate( model, glm::vec3( x, 0.0f, y) );
        } else {
            model = glm::translate( model, glm::vec3( 90.0f * scale, 0.0f, 0.0f) );
        }
        
        if (cameraType == "Earth") {
            camera.SetPosition(glm::vec3(x + 0.5f, -0.5f, y+0.5f));
        }
        
        model = glm::scale( model, glm::vec3( 0.5f * scale ) );
        angle = 0.001f * i;
        model = glm::rotate( model, angle, glm::vec3( 0.0f, 0.1f, 0.0f ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        earthModel.Draw( shader );
        
        // MARS
        model = glm::mat4(1);
        if (move) {
            angle = 0.005f * i  * speed;
            radius = 100.0f * scale;
            x = radius * sin(PI * 2 * angle / 360);
            y = radius * cos(PI * 2 * angle / 360);
            model = glm::translate( model, glm::vec3( x, 0.0f, y) );
        } else {
           model = glm::translate( model, glm::vec3( 100.0f * scale, 0.0f, 0.0f) );
        }
        
        if (cameraType == "Mars") {
            camera.SetPosition(glm::vec3(x + 0.5f, 0.0f, y+0.5f));
        }
        
        model = glm::scale( model, glm::vec3( 0.3f * scale ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        marsModel.Draw( shader );
        
        // JUPITER
        model = glm::mat4(1);
        if (move) {
            angle = 0.0045f * i  * speed;
            radius = 120.0f * scale;
            x = radius * sin(PI * 2 * angle / 360);
            y = radius * cos(PI * 2 * angle / 360);
            model = glm::translate( model, glm::vec3( x, 0.0f, y) );
        } else {
            model = glm::translate( model, glm::vec3( 120.0f * scale, 0.0f, 0.0f) );
        }
        
        if (cameraType == "Jupiter") {
            camera.SetPosition(glm::vec3(x + 0.5f, 0.0f, y+2.5f));
        }
        
        model = glm::scale( model, glm::vec3( 4.0f * scale ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        jupiterModel.Draw( shader );
        
        // SATURN
        model = glm::mat4(1);
        if (move) {
            angle = 0.0040f * i * speed;
            radius = 160.0f * scale;
            x = radius * sin(PI * 2 * angle / 360);
            y = radius * cos(PI * 2 * angle / 360);
            model = glm::translate( model, glm::vec3( x, 0.0f, y) );
        } else {
            model = glm::translate( model, glm::vec3( 160.0f * scale, 0.0f, 0.0f) );
        }
        
        if (cameraType == "Saturn") {
            camera.SetPosition(glm::vec3(x + 0.5f, 0.0f, y+2.5f));
        }
        
        model = glm::scale( model, glm::vec3( 0.032f * scale ) );
        angle = 0.0001f * i;
        model = glm::rotate( model, 90.0f + angle, glm::vec3( 0.0f, 0.1f, 0.5f ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        saturnModel.Draw( shader );
        
        // Uranus
        model = glm::mat4(1);
        if (move) {
            angle = 0.0035f * i * speed;
            radius = 190.0f * scale;
            x = radius * sin(PI * 2 * angle / 360);
            y = radius * cos(PI * 2 * angle / 360);
            model = glm::translate( model, glm::vec3( x, 0.0f, y) );
        } else {
            model = glm::translate( model, glm::vec3( 190.0f * scale, 0.0f, 0.0f) );
        }
        
        if (cameraType == "Uranus") {
            camera.SetPosition(glm::vec3(x + 0.5f, 0.0f, y+2.0f));
        }
        
        model = glm::scale( model, glm::vec3( 0.03f * scale ) );
        angle = 0.00001f * i;
        model = glm::rotate( model, 160.0f + angle, glm::vec3( 0.0f, 0.1f, 0.5f ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        uranusModel.Draw( shader );
        
        // NEPTUNE
        model = glm::mat4(1);
        if (move) {
            angle = 0.003f * i * speed;
            radius = 220.0f * scale;
            x = radius * sin(PI * 2 * angle / 360);
            y = radius * cos(PI * 2 * angle / 360);
            model = glm::translate( model, glm::vec3( x, 0.0f, y) );
        } else {
            model = glm::translate( model, glm::vec3( 220.0f * scale, 0.0f, 0.0f) );
        }
        
        if (cameraType == "Neptune") {
            camera.SetPosition(glm::vec3(x + 0.5f, 0.0f, y+2.0f));
        }
        
        model = glm::scale( model, glm::vec3( 0.03f * scale ) );
        angle = 0.00001f * i;
        model = glm::rotate( model, 130.0f + angle, glm::vec3( 0.0f, 0.1f, 0.5f ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        neptuneModel.Draw( shader );
        
        // SUN
        lampShader.Use( );
        GLint modelLoc = glGetUniformLocation( lampShader.Program, "model" );
        GLint viewLoc = glGetUniformLocation( lampShader.Program, "view" );
        GLint projLoc = glGetUniformLocation( lampShader.Program, "projection" );

        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );
        model = glm::mat4(1);
        model = glm::translate( model, lightPos );
        model = glm::scale( model, glm::vec3( 20.0f * scale  ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );
        sunModel.Draw( lampShader );
        
        if (cameraType == "Up") {
            camera.SetPosition(glm::vec3(-1.438195, 38.160343, 1.159209));
        }
        
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
};


void DoMovement() {
    
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] ) {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] ) {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] ) {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] ) {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
    
    if (keys[GLFW_KEY_MINUS]) {
        camera.DecreaseSpeed();
    }
    
    if (keys[GLFW_KEY_EQUAL]) {
        camera.IncreaseSpeed();
    }
    
    if (keys[GLFW_KEY_1]) {
        cameraType = "Mercury";
    } else if (keys[GLFW_KEY_2]) {
        cameraType = "Venus";
    } else if (keys[GLFW_KEY_3]) {
        cameraType = "Earth";
    } else if (keys[GLFW_KEY_4]) {
        cameraType = "Mars";
    } else if (keys[GLFW_KEY_5]) {
        cameraType = "Jupiter";
    } else if (keys[GLFW_KEY_6]) {
        cameraType = "Saturn";
    } else if (keys[GLFW_KEY_7]) {
        cameraType = "Uranus";
    } else if (keys[GLFW_KEY_8]) {
        cameraType = "Neptune";
    } else if (keys[GLFW_KEY_0]) {
        cameraType = "";
    } else if (keys[GLFW_KEY_U]) {
        cameraType = "Up";
    }
    
    if (keys[GLFW_KEY_M]) {
        speed = speed + 0.1f;
        std::cout << "SPEED : " << speed << std::endl;
    } else if (keys[GLFW_KEY_N]) {
        speed = speed - 0.1f;
        std::cout << "SPEED : " << speed << std::endl;
    }
}


void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode ) {
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action ) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 ) {
        if ( action == GLFW_PRESS ) {
            keys[key] = true;
        } else if ( action == GLFW_RELEASE ) {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos ) {
    if ( firstMouse ) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}
