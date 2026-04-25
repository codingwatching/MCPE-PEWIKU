#include "AppPlatform_unix.h"
#include "platform/log.h"
#include <png.h>
#include <fstream>

// Declare globals defined in main_unix.h
extern int g_winWidth;
extern int g_winHeight;

bool AppPlatform_unix::supportsTouchscreen()  { return false; }

TextureData AppPlatform_unix::loadTexture(const std::string& filename_, bool textureFolder) {
    TextureData out;
    std::string filename = textureFolder ? std::string("data/images/") + filename_ : filename_;
    std::ifstream source(filename.c_str(), std::ios::binary);
    if (source) {
        png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!pngPtr)
            return out;

        png_infop infoPtr = png_create_info_struct(pngPtr);
        if (!infoPtr) {
            png_destroy_read_struct(&pngPtr, NULL, NULL);
            return out;
        }

        png_set_read_fn(pngPtr, (png_voidp)&source, [](png_structp pngPtr, png_bytep data, png_size_t length) {
            ((std::istream*)png_get_io_ptr(pngPtr))->read((char*)data, length);
        });
        png_read_info(pngPtr, infoPtr);
        out.w = png_get_image_width(pngPtr, infoPtr);
        out.h = png_get_image_height(pngPtr, infoPtr);
        png_bytep* rowPtrs = new png_bytep[out.h];
        out.data = new unsigned char[4 * out.w * out.h];
        out.memoryHandledExternally = false;
        int rowStrideBytes = 4 * out.w;
        for (int i = 0; i < out.h; ++i) {
            rowPtrs[i] = (png_bytep)&out.data[i * rowStrideBytes];
        }
        png_read_image(pngPtr, rowPtrs);
        png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)0);
        delete[](png_bytep) rowPtrs;
        source.close();
        return out;
    } else {
        LOGI("Couldn't find file: %s\n", filename.c_str());
        return out;
    }
}

int AppPlatform_unix::getScreenWidth() {
    return g_winWidth;
}

int AppPlatform_unix::getScreenHeight() {
    return g_winHeight;
}

float AppPlatform_unix::getPixelsPerMillimeter() {
    return 4.0f;
}

void AppPlatform_unix::saveScreenshot(const std::string&, int, int) {
}

void AppPlatform_unix::finish() {
}