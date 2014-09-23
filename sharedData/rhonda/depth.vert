//
// Vertex shader for rendering a particle system
//
// Author: Randi Rost
//
// Copyright (c) 2003-2006: 3Dlabs, Inc.
//
// See 3Dlabs-License.txt for license information
//

//uniform float Time;            // updated each frame by the application
//uniform vec4  Background;      // constant color equal to background
// 
//attribute vec3  Velocity;      // initial velocity
//attribute float StartTime;     // time at which particle is activated
//
varying vec4 position;

void main(){
    gl_TexCoord[0] = gl_MultiTexCoord0;
    position = gl_Vertex;
    gl_Position  = gl_ModelViewProjectionMatrix * gl_Vertex;
}