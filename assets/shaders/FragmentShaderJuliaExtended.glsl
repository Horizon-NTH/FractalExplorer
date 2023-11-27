#version 460 core
precision highp float;

#define MAX_ITERATION 500
#define SMOOTH 100

out vec4 fragmentColor;

uniform vec2 canvasSize;

uniform dvec2 center;
uniform double offset;

layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;
const float PI = 3.1415926535897932384626433832795;

vec3 get_color_palette(float iteration);
vec3 get_color_HSV(float iteration);
vec3 get_color_HSL(float iteration);
vec3 get_color_LCH(float iteration);
vec3 get_color_wave(float iteration);
float get_continous_iteration(int iteration, vec2 complex);
vec3 get_iteration_mandelbrot();
vec3 get_iteration_julia();


void main()
{
    fragmentColor = vec4(get_iteration_mandelbrot(), 1.0);
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

vec3 get_color_LCH(float iteration)
{
	float v = 1.0 - pow(cos(PI * (iteration / MAX_ITERATION)), 2.0);
	vec3 lch = vec3(75.0 - 75.0 * v, 28.0 + (75.0 - 75.0 * v), mod(pow(360.0 * (iteration / MAX_ITERATION), 1.5), 360.0));
	float a = lch.y * cos(lch.z);
	float b = lch.y * sin(lch.z);
	float x = a / sqrt(3.0) + 1.0;
	float y = 1.0;
	float z = -b / sqrt(3.0) + 1.0;
	return vec3(y + 0.437 * (z - y), y - 0.062 * (x - y) - 0.109 * (z - y), y + 0.226 * (x - y));
}

vec3 get_color_wave(float iteration)
{
	return vec3((-cos(0.025*float(iteration))+1.0)/2.0,
                      (-cos(0.08*float(iteration))+1.0)/2.0,
                      (-cos(0.12*float(iteration))+1.0)/2.0);
}

float get_continous_iteration(int iteration, vec2 complex)
{
	if (iteration < MAX_ITERATION)
	{
		float nu = log((log(pow(complex.x, 2) + pow(complex.y, 2)) / 2.) / log(2))/log(2);
		return float(iteration + 1. - nu);
	}
	else
	{
		return iteration;
	}
}

vec3 get_iteration_mandelbrot()
{
	int iteration = 0;
	float aspectRatio = canvasSize.y / canvasSize.x;
	
	dvec2 complex = dvec2(
		mix(center.x - offset, center.x + offset, gl_FragCoord.x / canvasSize.x),
		mix(center.y - offset * aspectRatio, center.y + offset * aspectRatio, gl_FragCoord.y / canvasSize.y)
	);

	dvec2 magnitude = dvec2(0.0);

	dvec2 iterationComplex = dvec2(0.0, 0.0);

	while (magnitude.x + magnitude.y < SMOOTH && iteration < MAX_ITERATION)
	{	
		iterationComplex = dvec2(magnitude.x - magnitude.y, (iterationComplex.x + iterationComplex.x) * iterationComplex.y) + complex;
		magnitude = dvec2(iterationComplex.x * iterationComplex.x, iterationComplex.y * iterationComplex.y);
		iteration++;
	}
	if (iteration != MAX_ITERATION)
	{
        return vec3(get_color_HSL(get_continous_iteration(iteration, vec2(iterationComplex))));
	}
	else
		return vec3(0.0, 0.0, 0.0);
};

vec3 get_iteration_julia()
{
	int iteration = 0;
	float aspectRatio = canvasSize.y / canvasSize.x;
	
	vec2 iterationComplex = vec2(
		mix(center.x - offset, center.x + offset, gl_FragCoord.x / canvasSize.x),
		mix(center.y - offset * aspectRatio, center.y + offset * aspectRatio, gl_FragCoord.y / canvasSize.y)
	);

	vec2 complex = vec2(0.355, 0.355);

	while (pow(iterationComplex.x, 2) + pow(iterationComplex.y, 2) < SMOOTH && iteration < MAX_ITERATION)
	{	
		iterationComplex = vec2(pow(iterationComplex.x, 2) - pow(iterationComplex.y, 2), 2 * iterationComplex.x * iterationComplex.y) + complex;
		iteration++;
	}
	
	if (iteration != MAX_ITERATION)
	{
        return vec3(get_color_palette(get_continous_iteration(iteration, iterationComplex)));
	}
	else
		return vec3(0.0, 0.0, 0.0);
};
