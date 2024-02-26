#pragma once
//
// Created by Felix on 12/02/2024.
//

#include <vector>

#ifndef WAVECOLLAPSEFUNCTION_IMAGE_H
#define WAVECOLLAPSEFUNCTION_IMAGE_H

#endif //WAVECOLLAPSEFUNCTION_IMAGE_H

struct Color {
    float r,g,b;

    Color();
    Color(float r, float g, float b);
    Color(int r, int g, int b);
    ~Color();
};

class  Image{
public:
    Image(int width, int height);
    ~Image();

    [[nodiscard]] Color GetColor(int x, int y) const;
    void SetColor(const Color& color, int x, int y);

    void Export(const char* path) const;

    static void CreateTestImage();

private:
    int m_width;
    int m_height;
    std::vector<Color> m_colors;
};