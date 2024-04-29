//
// Created by Mrack on 2024/4/28.
//

#ifndef XPOSEDNHOOK_MODMENU_NATIVE_H
#define XPOSEDNHOOK_MODMENU_NATIVE_H

#include "imgui.h"
#include <GLES3/gl3.h>

void test_modmenu_native();

void input_inject();

void render();

class Window {
private:
    int ViewPort[4];

public:
    static Window *instance() {
        static Window *instance = NULL;
        if (instance == NULL) instance = new Window;
        return instance;
    }

    bool getViewPort() {
        glGetIntegerv(GL_VIEWPORT, this->ViewPort);
        if (this->ViewPort[2] && this->ViewPort[3]) return true;
        return false;
    }

    float getX() {
        return (float) this->ViewPort[0];
    }

    float getY() {
        return (float) this->ViewPort[1];
    }

    float getW() {
        return (float) this->ViewPort[2];
    }

    float getH() {
        return (float) this->ViewPort[3];
    }

    float getWidth() {
        return (this->getW() + (this->getX() * 2.0f));
    }

    float getHeight() {
        return (this->getH() + (this->getY() * 2.0f));
    }

    ImVec2 getSize() {
        return ImVec2(this->getWidth(), this->getHeight());
    }

    ImVec2 getCenter() {
        return ImVec2(this->getWidth() / 2.0f, this->getHeight() / 2.0f);
    }

    ImVec2 getScale() {
        return ImVec2(this->getWidth() * 0.00052083333f, this->getHeight() * 0.00092592592f);
    }

    bool isRotation() {
        if (this->getWidth() < this->getHeight()) return true;
        return false;
    }
};

#endif //XPOSEDNHOOK_MODMENU_NATIVE_H
