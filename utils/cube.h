
// A cube has 6 sides and each side has 4 vertices, therefore, the total number
// of vertices is 24 (6 sides * 4 verts), and 72 floats in the vertex array
// since each vertex has 3 components (x,y,z) (= 24 * 3)
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | v7----|-v4
//  |/      |/
//  v2------v3
//
// Vertex shader: the location (0: position attrib (vec3), 1: normal attrib (vec3),
//                              2: color attrib (vec4), and 3: texture coordinate attrib (vec2))
// Fragment shader: should catch the vertex color from the vertex shader

#ifndef CUBE_H
#define CUBE_H

#include "shader.h"

class Cube {
public:
    
    // vertex position array
    GLfloat cubeVertices[72]  = { // initialized as size = 1 for each dimension, 72 elements
        .5f, .5f, .5f,  -.5f, .5f, .5f,  -.5f,-.5f, .5f,  .5f,-.5f, .5f, // v0,v1,v2,v3 (front)
        .5f, .5f, .5f,   .5f,-.5f, .5f,   .5f,-.5f,-.5f,  .5f, .5f,-.5f, // v0,v3,v4,v5 (right)
        .5f, .5f, .5f,   .5f, .5f,-.5f,  -.5f, .5f,-.5f, -.5f, .5f, .5f, // v0,v5,v6,v1 (top)
        -.5f, .5f, .5f,  -.5f, .5f,-.5f,  -.5f,-.5f,-.5f, -.5f,-.5f, .5f, // v1,v6,v7,v2 (left)
        -.5f,-.5f,-.5f,   .5f,-.5f,-.5f,   .5f,-.5f, .5f, -.5f,-.5f, .5f, // v7,v4,v3,v2 (bottom)
        .5f,-.5f,-.5f,  -.5f,-.5f,-.5f,  -.5f, .5f,-.5f,  .5f, .5f,-.5f  // v4,v7,v6,v5 (back)
    };
    
    // normal array
    GLfloat cubeNormals[72] = {  // 72 elements
        0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,  // v0,v1,v2,v3 (front)
        1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,  // v0,v3,v4,v5 (right)
        0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,  // v0,v5,v6,v1 (top)
        -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  // v1,v6,v7,v2 (left)
        0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,  // v7,v4,v3,v2 (bottom)
        0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1   // v4,v7,v6,v5 (back)
    };
    
    // colour array
    GLfloat cubeColors[96] = { // initialized as RGBA sollid color for each face, 96 elements
        1, 0, 0, 1,   1, 0, 0, 1,   1, 0, 0, 1,   1, 0, 0, 1, // v0,v1,v2,v3 (front)
        1, 1, 0, 1,   1, 1, 0, 1,   1, 1, 0, 1,   1, 1, 0, 1, // v0,v3,v4,v5 (right)
        0, 1, 0, 1,   0, 1, 0, 1,   0, 1, 0, 1,   0, 1, 0, 1, // v0,v5,v6,v1 (top)
        0, 1, 1, 1,   0, 1, 1, 1,   0, 1, 1, 1,   0, 1, 1, 1, // v1,v6,v7,v2 (left)
        0, 0, 1, 1,   0, 0, 1, 1,   0, 0, 1, 1,   0, 0, 1, 1, // v7,v4,v3,v2 (bottom)
        1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1  // v4,v7,v6,v5 (back)
    };
    
    // texture coord array
    
    GLfloat cubeTexCoords[48] = { // 48 elements
        1, 1,   0, 1,   0, 0,   1, 0,               // v0,v1,v2,v3 (front)
        0, 1,   0, 0,   1, 0,   1, 1,               // v0,v3,v4,v5 (right)
        1, 0,   1, 1,   0, 1,   0, 0,               // v0,v5,v6,v1 (top)
        1, 1,   0, 1,   0, 0,   1, 0,               // v1,v6,v7,v2 (left)
        1, 1,   0, 1,   0, 0,   1, 0,               // v7,v4,v3,v2 (bottom)
        0, 0,   1, 0,   1, 1,   0, 1                // v4,v7,v6,v5 (back)
    };
    
    // index array for glDrawElements()
    // A cube requires 36 indices = 6 sides * 2 tris * 3 verts

    GLuint cubeIndices[36] = { // 36 elements
        0, 1, 2,   2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
        4, 5, 6,   6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
        8, 9, 10,  10, 11, 8,    // v0-v5-v6, v6-v1-v0 (top)
        12,13,14,  14,15,12,    // v1-v6-v7, v7-v2-v1 (left)
        16,17,18,  18,19,16,    // v7-v4-v3, v3-v2-v7 (bottom)
        20,21,22,  22,23,20     // v4-v7-v6, v6-v5-v4 (back)
    };

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    
    unsigned int vSize = sizeof(cubeVertices);
    unsigned int nSize = sizeof(cubeNormals);
    unsigned int cSize = sizeof(cubeColors);
    unsigned int tSize = sizeof(cubeTexCoords);
    
    Cube() {
        initBuffers();
    };
    
    Cube(float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        initBuffers();
    };
    
    Cube(float dx, float dy, float dz, float s) {
        scale(s);
        translate(dx, dy, dz);
        initBuffers();
    };
    
    Cube(float dx, float dy, float dz, float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        translate(dx, dy, dz);
        initBuffers();
    };
    
    void initBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize+nSize+cSize+tSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, cubeVertices);                  // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, cubeNormals);               // copy norms after verts
        glBufferSubData(GL_ARRAY_BUFFER, vSize+nSize, cSize, cubeColors);          // copy cols after norms
        glBufferSubData(GL_ARRAY_BUFFER, vSize+nSize+cSize, tSize, cubeTexCoords); // copy texs after cols
        
        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
        
        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)vSize); // normal attrib
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(vSize+nSize)); //color attrib
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(vSize+nSize+cSize)); // tex coord
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };
    
    void draw(Shader *shader) {
        shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };
    
    void translate(float dx, float dy, float dz) {
        for (int i = 0; i < 72; i++) {
            if (i % 3 == 0) cubeVertices[i] += dx;
            else if (i % 3 == 1) cubeVertices[i] += dy;
            else if (i % 3 == 2) cubeVertices[i] += dz;
        }
    };
    
    void scale(float s) {
        for (int i = 0; i < 72; i++)
            cubeVertices[i] *= s;
    };
    
    void scale2(float sx, float sy, float sz) {
        for (int i = 0; i < 72; i++) {
            if (i % 3 == 0) cubeVertices[i] *= sx;
            else if (i % 3 == 1) cubeVertices[i] *= sy;
            else if (i % 3 == 2) cubeVertices[i] *= sz;
        }
    }
};


#endif
