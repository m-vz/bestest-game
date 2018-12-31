//
// Created by Milan van Zanten on 19.04.18.
//

#ifndef BESTEST_GAME_FRAMEBUFFER_H
#define BESTEST_GAME_FRAMEBUFFER_H

#include "../texture/Texture.h"
#include "../../util/Log.h"
#include "../../exception/Exception.h"

class Framebuffer {
public:
    static const int MAX_COLOR_ATTACHMENTS = 8;

    Framebuffer(Texture *texture, GLenum attachment);
    Framebuffer(int width, int height);
    void addTexture(Texture *texture, GLenum attachment);
    void init(GLuint *drawAttachments = nullptr,
              int drawAttachmentsSize = 0,
              GLuint readAttachment = GL_NONE,
              bool readAttachmentSet = false,
              bool specifyTextarget = false);
    void init(bool drawColor, bool readColor, bool specifyTextarget = false);
    void bindFramebuffer(bool read);
    Texture *getColorTexture(int attachmentIndex) const;
    Texture *getDepthStencilTexture() const;
    Texture *getDepthTexture() const;
    int getFramebufferWidth() const;
    int getFramebufferHeight() const;
    virtual ~Framebuffer();

protected:
    bool initialised = false, depthStencilTextureBound = false, depthTextureBound = false;
    bool colorTexturesBound[MAX_COLOR_ATTACHMENTS]{};
    Texture *depthStencilTexture = nullptr, *depthTexture = nullptr;
    Texture *colorTextures[MAX_COLOR_ATTACHMENTS]{};
    GLuint fbo = 0;

    void checkFramebuffer(GLenum status);

private:
    int framebufferWidth, framebufferHeight;
};

#endif //BESTEST_GAME_FRAMEBUFFER_H
