#version 330 core

out vec4 ourColor;

uniform vec3 plotColor;

void main() {
    ourColor = vec4(plotColor, 1.0f);
}