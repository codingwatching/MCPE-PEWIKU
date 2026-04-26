#include "AppPlatform_unix.h"
#include "platform/log.h"
#include "world/level/storage/FolderMethods.h"
#include <SDL2/SDL.h>
#include <png.h>
#include <cstdio>
#include <fstream>

// Declare globals defined in main_unix.h
extern int g_winWidth;
extern int g_winHeight;

AppPlatform_unix::AppPlatform_unix() {
    char* basePath = SDL_GetBasePath();
    if (basePath && *basePath) {
        std::string executableDirectory(basePath);
        if (!executableDirectory.empty() && executableDirectory[executableDirectory.size() - 1] == '/') {
            executableDirectory.pop_back();
        }
        dataRoot = executableDirectory + "/data";
    }
    if (basePath) {
        SDL_free(basePath);
    }

    if (dataRoot.empty()) {
        dataRoot = "data";
    }
}

bool AppPlatform_unix::supportsTouchscreen()  { return false; }
bool AppPlatform_unix::supportsVibration() { return false; }
void AppPlatform_unix::vibrate(int milliSeconds) {}

TextureData AppPlatform_unix::loadTexture(const std::string& filename_, bool textureFolder) {
    TextureData out;
    std::string filename = textureFolder ? resolveAssetPath("images/" + filename_) : filename_;
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

        int color_type = png_get_color_type(pngPtr, infoPtr);
        int bit_depth  = png_get_bit_depth(pngPtr, infoPtr);

        if (bit_depth == 16) png_set_strip_16(pngPtr);
        if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(pngPtr);
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(pngPtr);
        if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(pngPtr);

        if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_filler(pngPtr, 0xFF, PNG_FILLER_AFTER);

        png_read_update_info(pngPtr, infoPtr);

        out.transparent = true;

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

BinaryBlob AppPlatform_unix::readAssetFile(const std::string& filename) {
    std::string assetPath = resolveAssetPath(filename);
    FILE* file = fopen(assetPath.c_str(), "rb");
    if (!file) {
        return BinaryBlob();
    }

    int size = getRemainingFileSize(file);
    BinaryBlob blob;
    blob.size = size;
    blob.data = new unsigned char[size];

    fread(blob.data, 1, size, file);
    fclose(file);
    return blob;
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

std::string AppPlatform_unix::resolveAssetPath(const std::string& relativePath) const {
    const std::string primaryPath = dataRoot + "/" + relativePath;
    std::ifstream primaryFile(primaryPath.c_str(), std::ios::binary);
    if (primaryFile.good()) {
        return primaryPath;
    }

    return "data/" + relativePath;
}
