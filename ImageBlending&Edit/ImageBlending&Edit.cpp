#include "EasyBMP.h"
#include <raylib.h>
#include <iostream>
#include<cmath>
#include <cstring>

const int IMG_NUMBER = 3;
struct TextTimer {
	const char* str;
	unsigned char time;

	TextTimer() : str(""), time(0) {}  // Default constructor
	TextTimer(const char* s, unsigned char t) : str(s), time(t) {}  // Constructor with parameters
};

struct Vector2i {
	int x;
	int y;
	Vector2 ToFloat() const {
		return Vector2{ (float)x, (float)y };
	}
};

struct pixel {
	float r, g, b, a;

	pixel() : r(0), g(0), b(0), a(1) {} // Default constructor with initialization
};

struct Sprite {
	pixel** PixelMap;
	int w, h;

	Sprite() : PixelMap(nullptr), w(0), h(0) {}  // Constructor to initialize members

	pixel** ToBW() {
		// Allocate memory for the new black and white pixel map
		pixel** pixelMapVar = new(std::nothrow) pixel * [w];
		if (!pixelMapVar) {
			std::cerr << "Memory allocation failed for PixelMap." << std::endl;
			exit(1);
		}

		for (int i = 0; i < w; ++i) {
			pixelMapVar[i] = new(std::nothrow) pixel[h];
			if (!pixelMapVar[i]) {
				std::cerr << "Memory allocation failed for PixelMap row." << std::endl;
				// Cleanup previously allocated memory before exiting
				for (int k = 0; k < i; ++k) {
					delete[] pixelMapVar[k];
				}
				delete[] pixelMapVar;
				exit(1);
			}
		}

		// Convert to black and white
		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < h; ++j) {
				// Calculate grayscale value
				float gray = PixelMap[i][j].r + PixelMap[i][j].g + PixelMap[i][j].b;
				//std::cout << gray << " " << i << " " << j << "\n";
				// Apply threshold for binary conversion
				if (gray <= 1.5f) {
					pixelMapVar[i][j].r = 0.0f;   // Red channel
					pixelMapVar[i][j].g = 0.0f;   // Green channel
					pixelMapVar[i][j].b = 0.502f; // Blue channel
				}
				else if (gray >= 1.5f) {
					pixelMapVar[i][j].r = 1.0f;     // Red channel
					pixelMapVar[i][j].g = 0.843f;   // Green channel
					pixelMapVar[i][j].b = 0.0f;     // Blue channel
				}
				pixelMapVar[i][j].a = PixelMap[i][j].a;  // Preserve the alpha channel
			}
		}

		return pixelMapVar;
	}
	pixel** ToGrayscale() {
		// Allocate memory for the new grayscale pixel map
		pixel** pixelMapVar = new(std::nothrow) pixel * [w];
		if (!pixelMapVar) {
			std::cerr << "Memory allocation failed for PixelMap." << std::endl;
			exit(1);
		}

		for (int i = 0; i < w; ++i) {
			pixelMapVar[i] = new(std::nothrow) pixel[h];
			if (!pixelMapVar[i]) {
				std::cerr << "Memory allocation failed for PixelMap row." << std::endl;
				// Cleanup previously allocated memory before exiting
				for (int k = 0; k < i; ++k) {
					delete[] pixelMapVar[k];
				}
				delete[] pixelMapVar;
				exit(1);
			}
		}

		// Convert to grayscale
		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < h; ++j) {
				// Calculate grayscale value using luminance formula
				float gray = 0.299f * PixelMap[i][j].r + 0.587f * PixelMap[i][j].g + 0.114f * PixelMap[i][j].b;

				// Set grayscale value for r, g, and b
				pixelMapVar[i][j].r = gray;
				pixelMapVar[i][j].g = gray;
				pixelMapVar[i][j].b = gray;
				pixelMapVar[i][j].a = PixelMap[i][j].a;  // Preserve the alpha channel
			}
		}

		return pixelMapVar;
	}
	pixel** ToRandFilter() {
		// Allocate memory for the new black and white pixel map
		pixel** pixelMapVar = new(std::nothrow) pixel * [w];
		if (!pixelMapVar) {
			std::cerr << "Memory allocation failed for PixelMap." << std::endl;
			exit(1);
		}

		for (int i = 0; i < w; ++i) {
			pixelMapVar[i] = new(std::nothrow) pixel[h];
			if (!pixelMapVar[i]) {
				std::cerr << "Memory allocation failed for PixelMap row." << std::endl;
				// Cleanup previously allocated memory before exiting
				for (int k = 0; k < i; ++k) {
					delete[] pixelMapVar[k];
				}
				delete[] pixelMapVar;
				exit(1);
			}
		}

		// Convert to black and white
		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < h; ++j) {
				float difference = std::abs(PixelMap[i][j - 1].r + PixelMap[i][j - 1].g + PixelMap[i][j - 1].b - PixelMap[i][j].r - PixelMap[i][j].g - PixelMap[i][j].b);
				// Apply threshold for binary conversion
				if (difference > 0.005f) {
					pixelMapVar[i][j].r = 0.0f;     // Red channel
					pixelMapVar[i][j].g = 0.0;   // Green channel
					pixelMapVar[i][j].b = 0.0f;   // Blue channel
				}
				else {
					pixelMapVar[i][j].r = pixelMapVar[i][j].g = pixelMapVar[i][j].b = 1;
				}
				pixelMapVar[i][j].a = PixelMap[i][j].a;  // Preserve the alpha channel
			}
		}

		return pixelMapVar;
	}
	pixel** toSobelEdgeDetection() {
		// Allocate memory for the new pixel map for storing Sobel edge detection output
		pixel** pixelMapVar = new(std::nothrow) pixel * [w];
		if (!pixelMapVar) {
			std::cerr << "Memory allocation failed for PixelMap." << std::endl;
			exit(1);
		}

		for (int i = 0; i < w; ++i) {
			pixelMapVar[i] = new(std::nothrow) pixel[h];
			if (!pixelMapVar[i]) {
				std::cerr << "Memory allocation failed for PixelMap row." << std::endl;
				// Cleanup previously allocated memory before exiting
				for (int k = 0; k < i; ++k) {
					delete[] pixelMapVar[k];
				}
				delete[] pixelMapVar;
				exit(1);
			}
		}

		// Sobel operator kernels for x and y gradients
		int Gx[3][3] = {
			{ -1, 0, 1 },
			{ -2, 0, 2 },
			{ -1, 0, 1 }
		};
		int Gy[3][3] = {
			{ -1, -2, -1 },
			{ 0, 0, 0 },
			{ 1, 2, 1 }
		};

		// Apply Sobel operator
		for (int i = 1; i < w - 1; ++i) {
			for (int j = 1; j < h - 1; ++j) {
				float gradX = 0.0f;
				float gradY = 0.0f;

				// Compute gradients in the x and y directions
				for (int k = -1; k <= 1; ++k) {
					for (int l = -1; l <= 1; ++l) {
						float intensity = 0.299f * PixelMap[i + k][j + l].r + 0.587f * PixelMap[i + k][j + l].g + 0.114f * PixelMap[i + k][j + l].b;
						gradX += Gx[k + 1][l + 1] * intensity;
						gradY += Gy[k + 1][l + 1] * intensity;
					}
				}

				// Calculate the magnitude of the gradient
				float magnitude = sqrt(gradX * gradX + gradY * gradY);

				// Normalize the magnitude to the range [0, 1]
				float normalizedMagnitude = magnitude / 4.0f;  // Max possible value is 4 for Sobel

				// Set pixel color based on the magnitude
				if (normalizedMagnitude > 0.0f) {
					// Edge detected - set to a nuance of gold
					// Adjust the intensity of gold based on the magnitude
					pixelMapVar[i][j].r = 1.0f * normalizedMagnitude;     // Red component of gold
					pixelMapVar[i][j].g = 0.843f * normalizedMagnitude;   // Green component of gold
					pixelMapVar[i][j].b = 0.0f;                          // Blue component stays 0
				}
				else {
					// No edge - set to dark red
					pixelMapVar[i][j].r = 0.0f;  // Red channel for dark red
					pixelMapVar[i][j].g = 0.0f;    // Green channel for dark red
					pixelMapVar[i][j].b = 0.0f;    // Blue channel for dark red
				}

				// Preserve the alpha channel
				pixelMapVar[i][j].a = PixelMap[i][j].a;
			}
		}

		return pixelMapVar;
	}
};

void AllocMat(Sprite& sprite) {
	sprite.PixelMap = new(std::nothrow) pixel * [sprite.w];
	if (!sprite.PixelMap) {
		std::cerr << "Memory allocation failed for PixelMap." << std::endl;
		exit(1);
	}

	for (int i = 0; i < sprite.w; ++i) {
		sprite.PixelMap[i] = new(std::nothrow) pixel[sprite.h];
		if (!sprite.PixelMap[i]) {
			std::cerr << "Memory allocation failed for PixelMap row." << std::endl;
			exit(1);
		}
	}
}

void ReadMat(Sprite& sprite, BMP& Img) {
	for (int i = 0; i < sprite.w; ++i) {
		for (int j = 0; j < sprite.h; ++j) {
			sprite.PixelMap[i][j].r = Img(i, j)->Red / 255.0f;
			sprite.PixelMap[i][j].g = Img(i, j)->Green / 255.0f;
			sprite.PixelMap[i][j].b = Img(i, j)->Blue / 255.0f;
			sprite.PixelMap[i][j].a = 1.0f; // Default alpha value
		}
	}
}

Vector2i OutputSize(Sprite sprite[]) {
	int LargestX = 0;
	int LargestY = 0;
	for (int i = 0; i < IMG_NUMBER; i++) {
		if (sprite[i].w > LargestX) LargestX = sprite[i].w;
		if (sprite[i].h > LargestY) LargestY = sprite[i].h;
	}
	return Vector2i{ LargestX, LargestY };
}

void WriteFile(const Sprite& sprite) {
	BMP Output;
	Vector2 outputSize{ sprite.w, sprite.h };
	Output.SetSize(outputSize.x, outputSize.y);
	Output.SetBitDepth(24);

	for (int i = 0; i < outputSize.x; ++i) {
		for (int j = 0; j < outputSize.y; ++j) {
			Output(i, j)->Red = static_cast<unsigned char>(sprite.PixelMap[i][j].r * 255);
			Output(i, j)->Green = static_cast<unsigned char>(sprite.PixelMap[i][j].g * 255);
			Output(i, j)->Blue = static_cast<unsigned char>(sprite.PixelMap[i][j].b * 255);
			Output(i, j)->Alpha = static_cast<unsigned char>(sprite.PixelMap[i][j].a * 255);
		}
	}
	Output.WriteToFile("MARBLES2.bmp");
}

void ChangeAlphaVal(Sprite& sprite, float alpha) {
	for (int i = 0; i < sprite.w; ++i) {
		for (int j = 0; j < sprite.h; ++j) {
			sprite.PixelMap[i][j].a = alpha / 255.0f; // Scale alpha to [0, 1]
		}
	}
}

pixel BlendPixel(const pixel& fg, const pixel& bg) {
	pixel blended;
	blended.a = fg.a + bg.a * (1 - fg.a);
	if (blended.a > 0) {
		blended.r = (fg.r * fg.a + bg.r * bg.a * (1 - fg.a)) / blended.a;
		blended.g = (fg.g * fg.a + bg.g * bg.a * (1 - fg.a)) / blended.a;
		blended.b = (fg.b * fg.a + bg.b * bg.a * (1 - fg.a)) / blended.a;
	}
	else {
		blended.r = blended.g = blended.b = 0;  // Fully transparent
	}
	return blended;
}

Sprite AlphaBlending(Sprite sprite[]) {
	Sprite spriteVar; // Start with the first sprite
	Vector2i outputSize = OutputSize(sprite);
	spriteVar.w = outputSize.x;
	spriteVar.h = outputSize.y;
	AllocMat(spriteVar);
	for (int i = 0; i < outputSize.x; ++i) {
		for (int j = 0; j < outputSize.y; ++j) {
			pixel finalPixel = sprite[0].PixelMap[i][j];
			for (int k = 1; k < IMG_NUMBER; ++k) {
				if (i < sprite[k].w && j < sprite[k].h) {
					finalPixel = BlendPixel(sprite[k].PixelMap[i][j], sprite[0].PixelMap[i][j]);
				}
			}
			spriteVar.PixelMap[i][j] = finalPixel;
		}
	}
	return spriteVar;
}

void DrawSprite(pixel** pixelMap, Vector2i outputSize, Sprite sprite[], TextTimer Extra) {
	for (int i = 0; i < outputSize.x - 1; ++i) {
		for (int j = 0; j < outputSize.y - 1; ++j) {
			Color pixelColor = BLACK; // Default color to black

			pixelColor.r = static_cast<unsigned char>(pixelMap[i][j].r * 255);
			pixelColor.g = static_cast<unsigned char>(pixelMap[i][j].g * 255);
			pixelColor.b = static_cast<unsigned char>(pixelMap[i][j].b * 255);
			pixelColor.a = static_cast<unsigned char>(pixelMap[i][j].a * 255);

			DrawPixel(i, j, pixelColor);
		}
	}
	if (Extra.time != 0) {
		Extra.time--;
		DrawText(Extra.str, outputSize.x - 550, outputSize.y - 50, 20, RED);
	}
	for (int i = 0; i < IMG_NUMBER; i++) {
		DrawText(TextFormat("Sprite %i: \n width: %i\n height: %i\n alpha value: %f", i, sprite[i].w, sprite[i].h, sprite[i].PixelMap[1][1].a * 255), (int)outputSize.x - 100, 60 * i + 10, 10, WHITE);
	}
}

void ScreenOutput(Sprite sprite[], Sprite& finalSprite) {
	bool img_efx[] = { false/*Black&White B*/,false/*Grayscale G*/,false/*Extra S*/,false/*Sobel Edge Detection E*/ };
	TextTimer Extra;
	unsigned char img_num = 0;
	unsigned char alpha_val = 100;
	Vector2i outputSize = OutputSize(sprite);

	InitWindow(outputSize.x, outputSize.y, "Raylib Program");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		bool actionOccurred = false;  // Track if any key interaction occurs

		if (IsKeyDown(KEY_RIGHT) && img_num < IMG_NUMBER - 1) {
			img_num++;
			Extra = TextTimer{ TextFormat("Image %i has been selected", (int)img_num), 100 };
			actionOccurred = true;
		}
		else if (IsKeyDown(KEY_LEFT) && img_num > 0) {
			img_num--;
			Extra = TextTimer{ TextFormat("Image %i has been selected", (int)img_num), 100 };
			actionOccurred = true;
		}

		if (IsKeyDown(KEY_UP) && alpha_val < 255) {
			alpha_val = std::min(alpha_val + 15, 255);
			ChangeAlphaVal(sprite[img_num], alpha_val);
			finalSprite = AlphaBlending(sprite);
			Extra = TextTimer{ TextFormat("Image %i alpha value has been changed to: %i", (int)img_num, (int)alpha_val), 100 };
			actionOccurred = true;
		}
		else if (IsKeyDown(KEY_DOWN) && alpha_val > 0) {
			alpha_val = std::max(alpha_val - 15, 0);
			ChangeAlphaVal(sprite[img_num], alpha_val);
			finalSprite = AlphaBlending(sprite);
			Extra = TextTimer{ TextFormat("Image %i alpha value has been changed to: %i", (int)img_num, (int)alpha_val), 100 };
			actionOccurred = true;
		}

		if (IsKeyDown(KEY_A)) {
			finalSprite = AlphaBlending(sprite);
			Extra = TextTimer{ TextFormat("Alpha (normal) blending has been applied"), 100 };
			actionOccurred = true;
		}

		if (IsKeyDown(KEY_B)) {
			img_efx[0] = !img_efx[0];
			Extra = TextTimer{ (img_efx[0]) ? "B&W filter has been enabled" : "B&W filter has been disabled", 100 };
			actionOccurred = true;
		}if (IsKeyDown(KEY_G)) {
			img_efx[1] = !img_efx[1];
			Extra = TextTimer{ (img_efx[1]) ? "Grayscale filter has been enabled" : "Grayscale filter has been disabled", 100 };
			actionOccurred = true;
		}
		if (IsKeyDown(KEY_S)) {
			img_efx[2] = !img_efx[2];
			Extra = TextTimer{ (img_efx[2]) ? "Rand filter has been enabled" : "Rand filter has been disabled", 100 };
			actionOccurred = true;
		}
		if (IsKeyDown(KEY_E)) {
			img_efx[3] = !img_efx[3];
			Extra = TextTimer{ (img_efx[3]) ? "Sobel Edge Detectionfilter has been enabled" : "Sobel Edge Detection filter has been disabled", 100 };
			actionOccurred = true;
		}
		// Reset the text if no action occurred
		if (!actionOccurred && Extra.time == 0) {
			Extra.str = "";  // Reset to an empty string
		}

		BeginDrawing();
		ClearBackground(BLACK);

		if (img_efx[0]) {
			DrawSprite(finalSprite.ToBW(), outputSize, sprite, Extra);
		}
		else if (img_efx[1]) {
			DrawSprite(finalSprite.ToGrayscale(), outputSize, sprite, Extra);
		}
		else if (img_efx[2]) {
			DrawSprite(finalSprite.ToRandFilter(), outputSize, sprite, Extra);
		}
		else if (img_efx[3]) {
			DrawSprite(finalSprite.toSobelEdgeDetection(), outputSize, sprite, Extra);
		}
		else {
			DrawSprite(finalSprite.PixelMap, outputSize, sprite, Extra);
		}

		EndDrawing();
	}

	CloseWindow();
}

int main() {
	Sprite sprite[IMG_NUMBER];
	Sprite outputSprite;
	BMP imageFile[IMG_NUMBER];
	if (!imageFile[0].ReadFromFile("Marbles.bmp")) {
		std::cerr << "Error: Could not open the image file Dog1.bmp!" << std::endl;
		return 1;
	}
	if (!imageFile[1].ReadFromFile("sample1.bmp")) {
		std::cerr << TextFormat("Error: Could not open the image file Flower%i.bmp", 1) << std::endl;
		return 1;
	}
	if (!imageFile[2].ReadFromFile("sample3.bmp")) {
		std::cerr << TextFormat("Error: Could not open the image file Flower%i.bmp", 1) << std::endl;
		return 1;
	}
	//for (int i = 1; i < IMG_NUMBER; i++)
	//	if (!imageFile[i].ReadFromFile(TextFormat("Flower%i.bmp", i))) {
	//		std::cerr << TextFormat("Error: Could not open the image file Flower%i.bmp", i) << std::endl;
	//		return 1;
	//	}

	for (int i = 0; i < IMG_NUMBER; i++) {
		sprite[i].w = imageFile[i].TellWidth();
		sprite[i].h = imageFile[i].TellHeight();

		AllocMat(sprite[i]);
		ReadMat(sprite[i], imageFile[i]);
	}

	outputSprite = sprite[0];
	ScreenOutput(sprite, outputSprite);
	WriteFile(outputSprite);
	return 0;
}