//
// Fragment shader for rendering a particle system
//
// Author: Randi Rost
//
// Copyright (c) 2003-2006: 3Dlabs, Inc.
//
// See 3Dlabs-License.txt for license information
//
varying vec4 position;
uniform float fakeDepthAdder;
uniform sampler2DRect ink;

void main (void)
{
    vec2 st = gl_TexCoord[0].st;
    vec4 inkColor = texture2DRect(ink, st);
    gl_FragColor = vec4(inkColor.x, inkColor.y, inkColor.z, 1.0-inkColor.x);
    gl_FragDepth = position.z+fakeDepthAdder;
}