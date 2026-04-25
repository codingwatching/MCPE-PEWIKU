#ifndef APPPLATFORM_UNIX_H__
#define APPPLATFORM_UNIX_H__

#include "AppPlatform.h"
#include <string>

class AppPlatform_unix : public AppPlatform {
public:
    AppPlatform_unix();

    bool supportsTouchscreen() override;
    TextureData loadTexture(const std::string& filename, bool textureFolder) override;
    BinaryBlob readAssetFile(const std::string& filename) override;
    int getScreenWidth() override;
    int getScreenHeight() override;
    float getPixelsPerMillimeter() override;
    void saveScreenshot(const std::string& filename, int width, int height) override;
    void finish() override;

private:
    std::string dataRoot;

    std::string resolveAssetPath(const std::string& relativePath) const;
};

#endif
