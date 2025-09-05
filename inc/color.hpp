#pragma once
#include <cmath>

struct color_t
{
    int r, g, b, a;

    constexpr color_t() : r(0), g(0), b(0), a(255) {}

    constexpr color_t(int r, int g, int b, int alpha = 255)
        : r{ r }, g{ g }, b{ b }, a{ alpha } {
    }

    constexpr color_t& operator *=(const float coeff)
    {
        this->r = static_cast<int>(this->r * coeff);
        this->g = static_cast<int>(this->g * coeff);
        this->b = static_cast<int>(this->b * coeff);
        return *this;
    }

    constexpr color_t operator ()(const int a) const
    {
        return color_t(r, g, b, a);
    }

    constexpr color_t& operator /=(const float div)
    {
        const auto flDiv = 1.f / div;
        *this *= flDiv;
        return *this;
    }

    constexpr color_t& operator *(const float coeff) const
    {
        auto color = *this;
        return color *= coeff;
    }

    constexpr float to_float(float v) {
        return v / 255.f;
    }

    constexpr color_t& from_hsv(float h, float s, float v)
    {
        float colOut[3]{ };
        if (s == 0.0f)
        {
            r = g = b = static_cast<int>(v * 255);
            return *this;
        }

        h = fmodf(h, 1.0f) / (60.0f / 360.0f);
        int   i = static_cast<int>(h);
        float f = h - static_cast<float>(i);
        float p = v * (1.0f - s);
        float q = v * (1.0f - s * f);
        float t = v * (1.0f - s * (1.0f - f));

        switch (i)
        {
        case 0:
            colOut[0] = v;
            colOut[1] = t;
            colOut[2] = p;
            break;
        case 1:
            colOut[0] = q;
            colOut[1] = v;
            colOut[2] = p;
            break;
        case 2:
            colOut[0] = p;
            colOut[1] = v;
            colOut[2] = t;
            break;
        case 3:
            colOut[0] = p;
            colOut[1] = q;
            colOut[2] = v;
            break;
        case 4:
            colOut[0] = t;
            colOut[1] = p;
            colOut[2] = v;
            break;
        case 5: default:
            colOut[0] = v;
            colOut[1] = p;
            colOut[2] = q;
            break;
        }

        r = static_cast<int>(colOut[0] * 255);
        g = static_cast<int>(colOut[1] * 255);
        b = static_cast<int>(colOut[2] * 255);
        return *this;
    }

    constexpr auto to_hsv(float& h, float& s, float& v)
    {
        float col[3] = { r / 255.f, g / 255.f, b / 255.f };

        float K = 0.f;
        if (col[1] < col[2])
        {
            swap(col[1], col[2]);
            K = -1.f;
        }
        if (col[0] < col[1])
        {
            swap(col[0], col[1]);
            K = -2.f / 6.f - K;
        }

        const float chroma = col[0] - (col[1] < col[2] ? col[1] : col[2]);
        h = colfabs(K + (col[1] - col[2]) / (6.f * chroma + 1e-20f));
        s = chroma / (col[0] + 1e-20f);
        v = col[0];
    }


    static constexpr color_t black(int a = 255) { return { 0, 0, 0, a }; }
    static constexpr color_t grey(int  a = 255) { return { 127, 127, 127, a }; }
    static constexpr color_t white(int a = 255) { return { 255, 255, 255, a }; }
    static constexpr color_t red(int   a = 255) { return { 255, 0, 0, a }; }
    static constexpr color_t green(int a = 255) { return { 0, 255, 0, a }; }
    static constexpr color_t blue(int  a = 255) { return { 0, 0, 255, a }; }

private:
    constexpr void  swap(float& a, float& b) { float tmp = a; a = b; b = tmp; }
    constexpr float colfabs(const float& x) { return x < 0 ? x * -1 : x; }

};
