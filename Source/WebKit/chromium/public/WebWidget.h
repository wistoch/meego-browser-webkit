/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WebWidget_h
#define WebWidget_h

#include "WebCanvas.h"
#include "WebCommon.h"
#include "WebCompositionUnderline.h"
#include "WebTextInputType.h"
#include "WebTextDirection.h"
#include "WebRect.h"

namespace WebKit {

class WebInputEvent;
class WebString;
struct WebPoint;
struct WebRect;
struct WebSize;
template <typename T> class WebVector;

class WebWidget {
public:
    // This method closes and deletes the WebWidget.
    virtual void close() = 0;

    // Returns the current size of the WebWidget.
    virtual WebSize size() = 0;

    // Called to resize the WebWidget.
    virtual void resize(const WebSize&) = 0;

    // Called to update imperative animation state.  This should be called before
    // paint, although the client can rate-limit these calls.
    virtual void animate() = 0;

    // set preferred size
    virtual void setPreferredContentsSize(const WebSize&) = 0;
    virtual WebSize preferredContentsSize() = 0;

    // set viewport size
    virtual void setViewportSize(const WebSize&) = 0;
    virtual WebSize viewportSize() = 0;

    // set actual contents rect
    virtual void setActualVisibleContentRect(const WebRect& actualVisibleContentRect) {}

#if defined(TOOLKIT_MEEGOTOUCH)
    virtual WebRect actualVisibleContentRect() { return WebRect(); };
#endif
    
    // Called to layout the WebWidget.  This MUST be called before Paint,
    // and it may result in calls to WebWidgetClient::didInvalidateRect.
    virtual void layout() = 0;

    // Called to paint the rectangular region within the WebWidget 
    // onto the specified canvas at (viewPort.x,viewPort.y). You MUST call
    // Layout before calling this method.  It is okay to call paint
    // multiple times once layout has been called, assuming no other
    // changes are made to the WebWidget (e.g., once events are
    // processed, it should be assumed that another call to layout is
    // warranted before painting again).
    virtual void paint(WebCanvas*, const WebRect& viewPort) = 0;

    // Triggers compositing of the current layers onto the screen.
    // The finish argument controls whether the compositor will wait for the
    // GPU to finish rendering before returning. You MUST call Layout
    // before calling this method, for the same reasons described in
    // the paint method above.
    virtual void composite(bool finish) = 0;

    // Called to inform the WebWidget of a change in theme.
    // Implementors that cache rendered copies of widgets need to re-render
    // on receiving this message
    virtual void themeChanged() = 0;

    // Called to inform the WebWidget of an input event.  Returns true if
    // the event has been processed, false otherwise.
    virtual bool handleInputEvent(const WebInputEvent&) = 0;

    // Called to inform the WebWidget that mouse capture was lost.
    virtual void mouseCaptureLost() = 0;

    // Called to inform the WebWidget that it has gained or lost keyboard focus.
    virtual void setFocus(bool) = 0;

    // Called to inform the WebWidget of a new composition text.
    // If selectionStart and selectionEnd has the same value, then it indicates
    // the input caret position. If the text is empty, then the existing
    // composition text will be cancelled.
    // Returns true if the composition text was set successfully.
    virtual bool setComposition(
        const WebString& text,
        const WebVector<WebCompositionUnderline>& underlines,
        int selectionStart,
        int selectionEnd) = 0;

    // Called to inform the WebWidget to confirm an ongoing composition.
    // This method is same as confirmComposition(WebString());
    // Returns true if there is an ongoing composition.
    virtual bool confirmComposition() = 0;

    // Called to inform the WebWidget to confirm an ongoing composition with a
    // new composition text. If the text is empty then the current composition
    // text is confirmed. If there is no ongoing composition, then deletes the
    // current selection and inserts the text. This method has no effect if
    // there is no ongoing composition and the text is empty.
    // Returns true if there is an ongoing composition or the text is inserted.
    virtual bool confirmComposition(const WebString& text) = 0;

    // Returns the current text input type of this WebWidget.
    virtual WebTextInputType textInputType() = 0;

    // Returns the current caret bounds of this WebWidget. The selection bounds
    // will be returned if a selection range is available.
    virtual WebRect caretOrSelectionBounds() = 0;

    // Returns the start and end point for the current selection, aligned to the
    // bottom of the selected line.
    virtual bool selectionRange(WebPoint& start, WebPoint& end) const = 0;

    // Changes the text direction of the selected input node.
    virtual void setTextDirection(WebTextDirection) = 0;

    // Returns true if the WebWidget uses GPU accelerated compositing
    // to render its contents.
    virtual bool isAcceleratedCompositingActive() const = 0;

#if defined(TOOLKIT_MEEGOTOUCH)
    enum NodeInfo {
      NODE_INFO_NONE = 0x00,
      NODE_INFO_IS_EMBEDDED_OBJECT = 0x01,
      NODE_INFO_IS_EDITABLE = 0x01 << 1,
      NODE_INFO_HAS_TOUCH_LISTENER = 0x01 << 2,
      NODE_INFO_IS_SCROLLABLE_AREA = 0x01 << 3,
      NODE_INFO_HAS_MOUSEMOVE_LISTENER = 0x01 << 4,
      NODE_INFO_HAS_DOUBLECLICK_LISTENER = 0x01 << 5,
      NODE_INFO_HAS_MOUSEOVER_LISTENER = 0x01 << 6,
      NODE_INFO_HAS_MOUSEOUT_LISTENER = 0x01 << 7,
      NODE_INFO_IS_NOT_SCALABLE_PAGE = 0x01 << 8
    };

    virtual void queryNodeTypeAtPoint(int x, int y, unsigned int& node_info) {};
    virtual void zoom2TextPre(int x, int y) {};
    virtual void zoom2TextPost() {};
    static void setUseExternalPopupMenus(bool);
    virtual void queryEditorCursorPosition(int& cursor_position) {};
    virtual void queryEditorCurrentSelection(WebString& selection) {};
    virtual void queryEditorSurroundingText(WebString& surrounding_text) {};
    virtual WebRect queryElementAreaAt(int x, int y, int max_width, int max_height){ return WebRect(); };
#endif

protected:
    ~WebWidget() { }
};

} // namespace WebKit

#endif
