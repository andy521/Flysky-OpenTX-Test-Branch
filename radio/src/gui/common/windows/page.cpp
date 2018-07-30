/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "opentx.h"

PageHeader::PageHeader(Page * parent, const rect_t & rect):
  Window(parent, rect, OPAQUE),
  back(this, { 0, 0, TOPBAR_BUTTON_WIDTH, TOPBAR_BUTTON_WIDTH }, ICON_BACK,
       [=]() -> uint8_t {
         parent->deleteLater();
         return 0;
       }, BUTTON_NOFOCUS)
{
}

void PageHeader::paint(BitmapBuffer * dc)
{
  dc->drawSolidFilledRect(TOPBAR_BUTTON_WIDTH, 0, LCD_W - TOPBAR_BUTTON_WIDTH, TOPBAR_BUTTON_WIDTH, HEADER_BGCOLOR);
}

Page::Page():
  Window(&mainWindow, {0, 0, LCD_W, LCD_H}, OPAQUE),
  header(this, {0, 0, LCD_W, headerHeight}),
  body(this, {0, headerHeight, LCD_W, LCD_H - headerHeight})
{
}

Page::~Page()
{
  TextKeyboard::instance()->disable();
  NumberKeyboard::instance()->disable();
  CurveKeyboard::instance()->disable();
}

void Page::paint(BitmapBuffer * dc)
{
  dc->clear(TEXT_BGCOLOR);
}

bool Page::onTouchEnd(coord_t x, coord_t y)
{
  if (Window::onTouchEnd(x, y))
    return true;
  TextKeyboard::instance()->disable();
  NumberKeyboard::instance()->disable();
  CurveKeyboard::instance()->disable();
  return true;
}
