//
//  camera.h
//  COSC3000
//
//  Created by Keyne Kassapa on 26/5/19.
//  Copyright © 2019 Keyne Kassapa. All rights reserved.
//

#pragma once
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>
#include <camera.h>

Camera camera( glm::vec3( 20.0f, 0.0f, 20.0f ));

void UFOMovement( Camera_Movement direction, GLfloat deltaTime ) {
    GLfloat speed = camera.GetMovementSpeed();
    glm::vec3 position = camera.GetPosition();
    GLfloat velocity = speed * deltaTime;
    if ( direction == FORWARD ) {
        position += camera.GetFront() * velocity;
    } else if ( direction == BACKWARD ) {
        position -= camera.GetFront() * velocity;
    } else if ( direction == LEFT ) {
        this->position -= this->right * velocity;
    } else if ( direction == RIGHT ) {
        this->position += this->right * velocity;
    } else if ( direction == UP ) {
        this->position += this->up * velocity;
    } else if ( direction == DOWN ) {
        this->position -= this->up * velocity;
    }
}

void ProcessMouseMovement( GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true ) {
    xOffset *= this->mouseSensitivity;
    yOffset *= this->mouseSensitivity;
    
    std::cout << xOffset << std::endl;
    
    this->yaw   += xOffset;
    this->pitch += yOffset;
    
    if ( constrainPitch ) {
        if ( this->pitch > 89.0f ) {
            this->pitch = 89.0f;
        }
        
        if ( this->pitch < -89.0f ) {
            this->pitch = -89.0f;
        }
    }
    this->updateCameraVectors( );
}

void SetPosition(glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f )) {
    this->position = position;
}

void DecreaseSpeed() {
    this->movementSpeed = this->movementSpeed - 0.01f;
    std::cout << "CAMERA SPEED : " << this->movementSpeed << std::endl;
}

void IncreaseSpeed() {
    this->movementSpeed = this->movementSpeed + 0.01f;
    std::cout << "CAMERA SPEED : " << this->movementSpeed << std::endl;
}

GLfloat GetZoom() {
    return this->zoom;
}

glm::vec3 GetPosition() {
    return this->position;
}

glm::vec3 GetFront() {
    return this->front;
}
    

