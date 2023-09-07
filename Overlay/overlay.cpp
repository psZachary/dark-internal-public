#include "pch.h"
#include "overlay.h"
#include <comdef.h>
#include <corecrt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mathtypes.h"

bool Overlay::init()
{
    // Taking this part out, for the pasters, I did not write this overlay, but modified it slightly. 

    return true;
}

bool Overlay::startup_d2d()
{
    // this stuff is all pretty basic

    RECT rc;
    if (HRESULT ret = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory) == S_OK)
    {
        if (ret = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) (&write_factory)) == S_OK)
        {

            write_factory->CreateTextFormat(font, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"en-us", &format);
            GetClientRect(window, &rc);

            if (ret = d2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(window, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), &tar) == S_OK)
            {
                D2D1_BRUSH_PROPERTIES properties = {1.0f, D2D1::Matrix3x2F::Identity()};

                tar->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), properties, &brush);

                return true;
            }
        }
    }
    return false;
}

void Overlay::begin_scene()
{
    tar->BeginDraw();
}

void Overlay::end_scene()
{
    tar->EndDraw();
}

void Overlay::clear_scene()
{
    tar->Clear();
}

void Overlay::draw_text(vector2 position, const char* text, D2D1::ColorF color, float size,...)
{
    char    buffer[4096];
    int     length;
    wchar_t out[256];
    RECT    window_metrics;

    if (!GetWindowRect(window, &window_metrics))
        return;

    va_list arg_list;
    va_start(arg_list, text);
    vsnprintf(buffer, sizeof(buffer), text, arg_list);
    va_end(arg_list);

    length = strlen(buffer);

    mbstowcs(out, buffer, length);
          
    brush->SetColor(color);
    this->size = size;
    tar->DrawTextW(out, length, format, D2D1::RectF(position.x, position.y, window_metrics.right - window_metrics.left, window_metrics.bottom - window_metrics.top), brush);
}

void Overlay::draw_text_outline(vector2 position, const char* text, D2D1::ColorF color, ...)
{
    char    buffer[4096];
    int     length;
    wchar_t out[256];
    RECT    window_metrics;

    auto outlineColor = D2D1::ColorF(0, 0, 0, 255);

    if (!GetWindowRect(window, &window_metrics))
        return;

    va_list arg_list;
    va_start(arg_list, text);
    vsnprintf(buffer, sizeof(buffer), text, arg_list);
    va_end(arg_list);

    length = strlen(buffer);

    mbstowcs(out, buffer, length);

    brush->SetColor(outlineColor);  // Set outline color
    tar->DrawTextW(out, length, format, D2D1::RectF(position.x - 1, position.y, window_metrics.right - window_metrics.left, window_metrics.bottom - window_metrics.top), brush); // Draw outline text (offset by 1 pixel)

    brush->SetColor(color);  // Set main text color
    tar->DrawTextW(out, length, format, D2D1::RectF(position.x, position.y, window_metrics.right - window_metrics.left, window_metrics.bottom - window_metrics.top), brush); // Draw main text
}

void Overlay::draw_line(vector2 f, vector2 t, D2D1::ColorF color, float stroke_width)
{
    brush->SetColor(color);
    tar->DrawLine(D2D1_POINT_2F{ f.x, f.y }, D2D1_POINT_2F{ t.x, t.y }, brush, stroke_width);

}
void Overlay::draw_circle(vector2 position, float radius, D2D1::ColorF color, float stroke_width)
{
    D2D1_ELLIPSE elipse{};
    elipse.radiusX = radius;
    elipse.radiusY = radius;
    elipse.point = D2D1_POINT_2F{ position.x, position.y };

    brush->SetColor(color);
    tar->DrawEllipse(elipse, brush, stroke_width);
}
void Overlay::draw_rect_filled(vector2 position, float width, float height, D2D1::ColorF color)
{
    D2D1_RECT_F rect = D2D1::RectF(position.x, position.y, position.x + width, position.y + height);

    brush->SetColor(color);
    tar->FillRectangle(rect, brush);
}
void Overlay::draw_rect(vector2 position, float width, float height, D2D1::ColorF color, float stroke_width)
{
    D2D1_RECT_F rect = D2D1::RectF(position.x, position.y, position.x + width, position.y + height);

    brush->SetColor(color);
    tar->DrawRectangle(rect, brush, stroke_width);
}
void Overlay::draw_rect(vector2 position1, vector2 position2, D2D1::ColorF color, float stroke_width)
{
    D2D1_RECT_F rect = D2D1::RectF(position1.x, position1.y, position2.x, position2.y);

    brush->SetColor(color);
    tar->DrawRectangle(rect, brush, stroke_width);
}
void Overlay::draw_rect_outline(vector2 position, float width, float height, D2D1::ColorF color, float stroke_width)
{
    auto outlineColor = D2D1::ColorF(0, 0, 0, 255);

    D2D1_RECT_F rect = D2D1::RectF(position.x, position.y, position.x + width, position.y + height);
    D2D1_RECT_F outline_rect = D2D1::RectF(position.x - 1, position.y, ( position.x - 1 ) + width, position.y + height);

    brush->SetColor(outlineColor);
    tar->DrawRectangle(outline_rect, brush, stroke_width);
    
    brush->SetColor(color);
    tar->DrawRectangle(rect, brush, stroke_width);
}