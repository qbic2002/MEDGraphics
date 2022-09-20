//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_VIEW_H
#define MEDGRAPHICS_VIEW_H

#include <functional>
#include "Context.h"
#include "../gl/Texture.h"
#include "../img/rgba.h"

namespace view {

    struct padding {
        float left;
        float top;
        float right;
        float bottom;

        explicit padding(float value) : padding(value, value) {
        }

        padding(float horizontal, float vertical) : left(horizontal), top(vertical), right(horizontal),
                                                    bottom(vertical) {
        }

        padding(float left, float top, float right, float bottom) : left(left), top(top), right(right),
                                                                    bottom(bottom) {}
    };

    class Background {
    public:
        rgba color{};
        rgba colorOnHover{};
        rgba colorOnPress{};

        Background& setImage(const std::string& fileName) {
            delete image;
            image = new gl::Texture(fileName);
            return *this;
        }

        Background& setColor(const rgba& _color) {
            Background::color = _color;
            return *this;
        }

        Background& setColorOnHover(const rgba& _colorOnHover) {
            Background::colorOnHover = _colorOnHover;
            return *this;
        }

        Background& setColorOnPress(const rgba& _colorOnPress) {
            Background::colorOnPress = _colorOnPress;
            return *this;
        }

        gl::Texture* getImage() const {
            return image;
        }

        ~Background() {
            delete image;
        }

    private:
        gl::Texture* image = nullptr;
    };

    enum State {
        DEFAULT,
        HOVERED,
        PRESSED
    };

    class View {
    public:
        View(Context* context, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

        View(Context* context) : View(context, 0, 0, 0, 0) {}

        unsigned int getX() const;

        unsigned int getY() const;

        unsigned int getWidth() const;

        unsigned int getHeight() const;

        void setPosition(unsigned int _x, unsigned int _y, unsigned int _width, unsigned int _height) {
            this->x = _x;
            this->y = _y;
            this->width = _width;
            this->height = _height;
        }

        void setPadding(const padding& _padding) {
            this->padding = _padding;
        }

        Background& getBackground() {
            return this->background;
        }

        void onClick() {
            if (onClickListener != nullptr) {
                onClickListener();
            }
        };

        void setOnClickListener(std::function<void()>&& _onClickListener) {
            onClickListener = _onClickListener;
        }

        virtual void render();

        virtual ~View() = default;

        State state = DEFAULT;

    protected:
        Context* context;
        unsigned int x, y, width, height;
        padding padding{0, 0, 0, 0};
        Background background;
        std::function<void()> onClickListener = nullptr;
    };

} // view

#endif //MEDGRAPHICS_VIEW_H
