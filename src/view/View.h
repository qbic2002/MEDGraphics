//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_VIEW_H
#define MEDGRAPHICS_VIEW_H

#include <functional>
#include "Context.h"
#include "../gl/Texture.h"
#include "../img/rgba.h"
#include "Style.h"
#include "ClickEvent.h"

namespace view {

    enum State {
        DEFAULT,
        HOVERED,
        PRESSED
    };

    class View {
    public:
        explicit View(Context* context) : context(context), style() {}

        View(Context* context, const Style& style) : context(context), style(style) {}

        View(Context* context, const Style&& style) : context(context), style(style) {}

        View(const View& other) = delete;

        View(const View&& other) = delete;

        View& operator=(const View& other) = delete;

        View& operator=(const View&& other) = delete;

        virtual ~View() = default;

        Style& getStyle();

        /// @return whether event was consumed
        virtual bool onClick(const ClickEvent& event);

        virtual bool onDrag(double x, double y, double dx, double dy);

        View& setOnClickListener(const std::function<void()>& _onClickListener);

        View& setOnClickListener(const std::function<void()>&& _onClickListener);

        virtual void onMeasure(const CalculatedPos& parentPos);

        virtual void onWindowResize(unsigned int width, unsigned int height);

        View& setOnWindowResizeListener(std::function<void(View& v, unsigned int w, unsigned int h)>&& _onWindowResize);

        virtual void onMouseEnter();

        virtual void onMouseLeave();

        virtual void onMouseMove(double x, double y);

        virtual bool onScroll(double xOffset, double yOffset, double d, double d1);

        virtual void renderBackground();

        virtual void render();

        virtual bool isInside(double x, double y);

        const Context* getContext() const;

        State state = DEFAULT;

    protected:
        Context* context;
        Style style;
        CalculatedPos calculatedPos{};
        std::function<void()> onClickListener = nullptr;
        std::function<void(View& view, unsigned int width, unsigned int height)> onWindowResizeListener = nullptr;
    };

} // view

#endif //MEDGRAPHICS_VIEW_H
