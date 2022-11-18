//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_VIEW_H
#define MEDGRAPHICS_VIEW_H

#include <functional>
#include <view/core/Context.h>
#include <utils/Texture.h>
#include <img/rgba.h>
#include <view/Style.h>
#include "ClickEvent.h"

namespace view {

    enum State {
        DEFAULT,
        HOVERED,
        PRESSED
    };

    class View {
    public:
        explicit View(Context* context);

        View(Context* context, const Style& style);

        View(Context* context, const Style&& style);

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

        int getId() const;

        View* setId(int _id);

        virtual View* findViewById(int _id);

        const StyleState* getStyleState() const;

        State getState() const;

        void setState(State _state);

        struct space_requirement {
            union {
                struct {
                    float width = 0;
                    float height = 0;
                    float parentPartW = 0;
                    float parentPartH = 0;
                    float parentSparePartW = 0;
                    float parentSparePartH = 0;
                };
                struct {
                    float size[2];
                    float parentPart[2];
                    float parentSparePart[2];
                };
            };
        };

        virtual space_requirement howMuchSpaceRequired() {
            return {0, 0, 0, 0, 0, 0};
        }

        virtual void useThisSpace(float left, float top, float right, float bottom) {
            edges.left = left;
            edges.top = top;
            edges.right = right;
            edges.bottom = bottom;
        }

    protected:
        Context* context;
        State state = DEFAULT;
        Style style;
        StyleState* styleState = &style.stateDefault;
        CalculatedPos calculatedPos{};
        std::function<void()> onClickListener = nullptr;
        std::function<void(View& view, unsigned int width, unsigned int height)> onWindowResizeListener = nullptr;
        int id = 0;

        struct edges {
            float left = 0;
            float top = 0;
            float right = 0;
            float bottom = 0;
        } edges;
    };

} // view

#endif //MEDGRAPHICS_VIEW_H
