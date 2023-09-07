#pragma once

#include <d2d1.h>
#include <dwmapi.h>
#include <dwrite.h>
#include <string>
#include <vector>
#include <map>
#include <windows.h>
#include "mathtypes.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Dwmapi.lib") 
#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Dwrite.lib") 

class Overlay
{
private:
    HWND window;

    ID2D1Factory*          d2d_factory;
    ID2D1HwndRenderTarget* tar;
    IDWriteFactory*        write_factory;
    ID2D1SolidColorBrush*  brush;
    IDWriteTextFormat*     format;

    const wchar_t* font;
    float          size;

public:


    Overlay(const wchar_t* Font, float Font_Size)
    {
        this->font = Font;
        this->size = Font_Size;
    }

    ~Overlay()
    {
        begin_scene();
        clear_scene();
        end_scene();

        tar->Release();
        write_factory->Release();
        brush->Release();
        d2d_factory->Release();

        printf("tar: %p, write: %p, brush: %p, factory: %p", tar, write_factory, brush, d2d_factory);
    }

    bool init();
    bool startup_d2d();

    void begin_scene();
    void end_scene();
    void clear_scene();

    /* create helper functions here */
    void draw_text(vector2 position, const char* text, D2D1::ColorF color, float size = 16.0f, ...);
    void draw_text_outline(vector2 position, const char* text, D2D1::ColorF color, ...);
    void draw_line(vector2 f, vector2 t, D2D1::ColorF color, float stroke_width = 1.0f);
    void draw_circle(vector2 position, float radius, D2D1::ColorF color, float stroke_width = 1.0f);
    void draw_rect_filled(vector2 position, float width, float height, D2D1::ColorF color);
    void draw_rect(vector2 position, float width, float height, D2D1::ColorF color, float stroke_width = 1.0f);
    void draw_rect(vector2 position1, vector2 position2, D2D1::ColorF color, float stroke_width);
    void draw_rect_outline(vector2 position, float width, float height, D2D1::ColorF color, float stroke_width);
};