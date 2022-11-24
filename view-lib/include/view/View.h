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
#include "../../../src/widget/WidgetAttributes.h"

namespace view {

    enum State {
        DEFAULT,
        HOVERED,
        PRESSED
    };

    struct ViewAttributes {
        VIEW_ATTRS
    };

    class View {
    public:
        View(Context* context, const ViewAttributes& attr) : context(context) {
            VIEW_ATTRS_SET(attr)
        }

        View(const View& other) = delete;

        View(const View&& other) = delete;

        View& operator=(const View& other) = delete;

        View& operator=(const View&& other) = delete;

        virtual ~View() = default;

        Style& getStyle() {
            return style;
        }

        /// @return whether event was consumed
        virtual bool onClick(const ClickEvent& event);

        virtual bool onDrag(double x, double y, double dx, double dy);

        View& setOnClickListener(const std::function<void()>& _onClickListener);

        virtual void onWindowResize(unsigned int width, unsigned int height);

        View& setOnWindowResizeListener(std::function<void(View& v, unsigned int w, unsigned int h)>&& _onWindowResize);

        virtual void onMouseEnter();

        virtual void onMouseLeave();

        virtual void onMouseMove(double x, double y);

        virtual bool onScroll(double xOffset, double yOffset, double x, double y);

        void draw();

        virtual void onDraw();

        virtual bool isInside(double x, double y);

        const Context* getContext() const;

        int getId() const;

        View* setId(int _id);

        virtual View* findViewById(int _id);

        State getState() const;

        void setState(State _state);

        struct SpaceRequirement {
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

        SpaceRequirement measure() {
            if (isMeasured)
                return measuredSpaceRequirement;
            measuredSpaceRequirement = onMeasure();
            isMeasured = true;
            return measuredSpaceRequirement;
        }

        virtual float getContentWidth() {
            return 0;
        }

        virtual float getContentHeight() {
            return 0;
        }

        virtual SpaceRequirement onMeasure() {
            return {width.evaluateContent(getContentWidth()) + padding.width(),
                    height.evaluateContent(getContentHeight()) + padding.height(),
                    width.parentK,
                    height.parentK,
                    width.parentSpareK,
                    height.parentSpareK};
        }

        void layout(float left, float top, float right, float bottom) {
            edges.left = left;
            edges.top = top;
            edges.right = right;
            edges.bottom = bottom;
            innerEdges.left = left + padding.left;
            innerEdges.top = top + padding.top;
            if (innerEdges.top == 153) {
                info() << "oh";
            }
            innerEdges.right = right - padding.right;
            innerEdges.bottom = bottom - padding.bottom;
            onLayout(left, top, right, bottom);
        }

        virtual void onLayout(float left, float top, float right, float bottom) {}

        void setParent(View* parent) {
            if (this->parent != nullptr) {
                throw std::runtime_error("View already has a parent");
            }
            this->parent = parent;
        }

        bool isDirty() const {
            return needRerender;
        }

        void invalidate() {
            isMeasured = false;
            needRerender = true;
            if (parent != nullptr) {
                parent->invalidate();
            }
        }

    protected:
        Context* context;
        State state = DEFAULT;
        std::function<void()> onClickListener = nullptr;
        std::function<void(View& view, unsigned int width, unsigned int height)> onWindowResizeListener = nullptr;

        VIEW_ATTRS

        struct edges {
            float left = 0;
            float top = 0;
            float right = 0;
            float bottom = 0;

            float width() const {
                return right - left;
            }

            float height() const {
                return bottom - top;
            }
        } edges, innerEdges;

        View* parent = nullptr;
    private:
        // Layout staff
        SpaceRequirement measuredSpaceRequirement{};
        bool isMeasured = false;
        bool needRerender = true;
    };

} // view

#endif //MEDGRAPHICS_VIEW_H
