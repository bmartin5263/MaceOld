#ifndef TEAPOTS_TEXTURE_H
#define TEAPOTS_TEXTURE_H

#include <Core.h>
#include "EGL/egl.h"
#include "GLES/gl.h"
#include "android/asset_manager.h"
#include "c++/v1/string"
#include "c++/v1/vector"

MACE_START

/**
 *  class Texture
 *    adding texture into teapot
 *     - oad image in assets/Textures
 *     - enable texture units
 *     - report samplers needed inside shader
 *  Functionality wise:
 *     - one texture
 *     - one sampler
 *     - texture unit 0, sampler unit 0
 */
class Texture {
  public:

    Texture() = default;
    virtual ~Texture() = default;
    /**
     *   Create a texture object
     * @param type  should be GL_TEXTURE_2D / GL_TEXTURE_CUBE_MAP
     * @param texFiles holds image file names under APK/assets.
     *     2d texture uses the very first image texFiles[0]
     *     cube map needs 6 (direction of +x, -x, +y, -y, +z, -z)
     * @param assetManager Java side assetManager object
     * @return newly created texture object, or nullptr in case of errors
     */
    static Texture* Create( GLuint type, std::vector<std::string>& texFiles,
              AAssetManager* assetManager);

    virtual bool GetActiveSamplerInfo(std::vector<std::string> &names,
                                      std::vector<GLint> &units) = 0;
    virtual bool Activate(void) = 0;
    virtual GLuint GetTexType() = 0;
    virtual GLuint GetTexId() = 0;

};
#endif //TEAPOTS_TEXTURE_H

MACE_END