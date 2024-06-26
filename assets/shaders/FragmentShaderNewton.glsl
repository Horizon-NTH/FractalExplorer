#version 330 core

#define MAX_ITERATION 100
#define SMOOTH 10e-6

out vec4 fragmentColor;

uniform vec2 canvasSize;
uniform vec2 center;
uniform float offset;
uniform int color;

layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;
const float PI = 3.1415926535897932384626433832795;

vec3 newton();
vec2 cmul(vec2 c1, vec2 c2);
vec2 cdiv(vec2 c1, vec2 c2);
vec3 get_color_palette(float iteration);
vec3 get_color_HSV(float iteration);
vec3 get_color_HSL(float iteration);
vec3 get_color_wave(float iteration);

void main()
{
    fragmentColor = vec4(newton(), 1.0);
}

vec3 newton()
{
    int iteration = 0;
    float aspectRatio = canvasSize.y / canvasSize.x;
    const vec2 root[3] = {
        vec2(1.0, 0.0),             // 1
        vec2(-0.5, 0.86602540378),  // -0.5 + i*sqrt(3)/2
        vec2(-0.5, -0.86602540378)  // -0.5 - i*sqrt(3)/2
    };
    float distance_to_root[3] = {
        length(root[0]),
        length(root[1]),
        length(root[2])
    };
    float distance_min = min(distance_to_root[0], min(distance_to_root[1], distance_to_root[2]));

    vec2 complex = vec2(
        mix(center.x - offset, center.x + offset, gl_FragCoord.x / canvasSize.x),
        mix(center.y - offset * aspectRatio, center.y + offset * aspectRatio, gl_FragCoord.y / canvasSize.y)
    );

    while (distance_min > SMOOTH && iteration < MAX_ITERATION)
    {
        vec2 squarred_complex = cmul(complex, complex);
        complex -= cdiv(cmul(squarred_complex, complex) - vec2(1.0, 0.0), 3.0 * squarred_complex);
        for (int i = 0; i < 3; i++)
        {
            distance_to_root[i] = length(complex - root[i]);
            if (distance_to_root[i] < distance_min)
                distance_min = distance_to_root[i];
        }
        iteration++;
    }
    if (iteration != MAX_ITERATION)
    {
        float continuous_iteration = float(iteration) - log2(log(max(distance_min, 10e-12)) / log(SMOOTH));
        switch (color)
        {
            case 0:
                float color_value = 0.6 + 0.4 * cos(0.25 * (continuous_iteration));
                if (iteration == MAX_ITERATION)
                    return vec3(0.f, 0.f, 0.f);
                else if (distance_to_root[0] < distance_to_root[1] && distance_to_root[0] < distance_to_root[2])
                    return vec3(1.f * color_value, 0.f, 0.3f * color_value);
                else if (distance_to_root[1] < distance_to_root[0] && distance_to_root[1] < distance_to_root[2])
                    return vec3(0.f, 1.f * color_value, 0.3f * color_value);
                return vec3(0.f, 0.3f * color_value, 1.f * color_value);
            case 1:
                return vec3(get_color_wave(continuous_iteration));
            case 2:
                return vec3(get_color_HSL(continuous_iteration));
            case 3:
                return vec3(get_color_HSV(continuous_iteration));
            default:
                return vec3(get_color_wave(continuous_iteration));
        }
    }
    return vec3(0.0, 0.0, 0.0);
}

vec2 cmul(vec2 c1, vec2 c2)
{
    return vec2(c1.x * c2.x - c1.y * c2.y, c1.x * c2.y + c1.y * c2.x);
}

vec2 cdiv(vec2 c1, vec2 c2)
{
    float d = c2.x * c2.x + c2.y * c2.y;
    return vec2((c1.x * c2.x + c1.y * c2.y) / d, (c1.y * c2.x - c1.x * c2.y) / d);
}

vec3 get_color_palette(float iteration)
{
    const int paletteSize = 100;
    vec3 palette[paletteSize];

    vec3 colorStart = vec3(0.2, 0.2, 0.8);
    vec3 colorEnd = vec3(0.4, 1.0, 1.0);

    for (int i = 0; i < paletteSize; i++)
    {
        float t = float(i) / float(paletteSize - 1);
        palette[i] = mix(colorStart, colorEnd, t);
    }

    vec3 color1 = palette[int(floor(iteration))];
    vec3 color2 = palette[int(floor(iteration) + 1)];
    return mix(color1, color2, fract(iteration));
}

vec3 get_color_HSV(float iteration)
{
    vec3 hsv = vec3(mod(pow((iteration / MAX_ITERATION) * 360, 1.5), 360.0), 100, (iteration / MAX_ITERATION) * 100);
    float c = hsv.z * hsv.z;
    float x = c * (1.0 - abs(mod(hsv.x / 60.0, 2.0) - 1.0));
    float m = hsv.z - c;
    if (hsv.x < 60.0)
    {
        return vec3(c, x, 0.0) + m;
    }
    else if (hsv.x < 120.0)
    {
        return vec3(x, c, 0.0) + m;
    }
    else if (hsv.x < 180.0)
    {
        return vec3(0.0, c, x) + m;
    }
    else if (hsv.x < 240.0)
    {
        return vec3(0.0, x, c) + m;
    }
    else if (hsv.x < 300.0)
    {
        return vec3(x, 0.0, c) + m;
    }
    else
    {
        return vec3(c, 0.0, x) + m;
    }
}

vec3 get_color_HSL(float iteration)
{
    vec3 hsl = vec3(mod(pow((iteration / MAX_ITERATION) * 360, 1.5), 360.0), 50, (iteration / MAX_ITERATION) * 100);
    float c = (1.0 - abs(2.0 * hsl.z - 1.0)) * hsl.y;
    float x = c * (1.0 - abs(mod(hsl.x / 60.0, 2.0) - 1.0));
    float m = hsl.z - c / 2.0;
    if (hsl.x < 60.0)
    {
        return vec3(c, x, 0.0) + m;
    }
    else if (hsl.x < 120.0)
    {
        return vec3(x, c, 0.0) + m;
    }
    else if (hsl.x < 180.0)
    {
        return vec3(0.0, c, x) + m;
    }
    else if (hsl.x < 240.0)
    {
        return vec3(0.0, x, c) + m;
    }
    else if (hsl.x < 300.0)
    {
        return vec3(x, 0.0, c) + m;
    }
    else
    {
        return vec3(c, 0.0, x) + m;
    }
}

vec3 get_color_wave(float iteration)
{
    return vec3((-cos(0.025*float(iteration))+1.0)/2.0,
        (-cos(0.08*float(iteration))+1.0)/2.0,
        (-cos(0.12*float(iteration))+1.0)/2.0);
}