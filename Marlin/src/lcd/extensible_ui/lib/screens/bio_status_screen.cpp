/*************************
 * bio_status_screen.cpp *
 *************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(EXTENSIBLE_UI) && defined(LULZBOT_USE_BIOPRINTER_UI)

#include "screens.h"

#include "../ftdi_eve_lib/extras/poly_ui.h"
#include "bio_printer_ui.h"

#define GRID_COLS 4
#define GRID_ROWS 9

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

static uint8_t increment;

void StatusScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(background));
  cmd.cmd(CLEAR(true,true,true));
  cmd.tag(0);

  #define POLY(A) PolyUI::poly_reader_t(A, sizeof(A)/sizeof(A[0]))

  PolyUI ui(cmd);

  // Paint the shadow for the syringe
  ui.color(shadow_rgb);
  ui.shadow(POLY(syringe_outline), 5);

  // Paint the syring icon
  ui.color(syringe_rgb);
  ui.fill(POLY(syringe_outline));
  ui.color(fill_rgb);
  ui.fill(POLY(syringe_fluid));
  ui.color(stroke_rgb);
  ui.fill(POLY(syringe));

  // Draw the arrow push buttons

  ui.button_fill  (fill_rgb);
  ui.button_stroke(stroke_rgb, 28);
  ui.button_shadow(shadow_rgb, 5);

  ui.button(1, POLY(x_neg));
  ui.button(2, POLY(x_pos));
  ui.button(3, POLY(y_neg));
  ui.button(4, POLY(y_pos));
  ui.button(5, POLY(z_neg));
  ui.button(6, POLY(z_pos));
  ui.button(7, POLY(e_neg));
  ui.button(8, POLY(e_pos));

  // Overlay FTDI buttons over the polygons
  // for the buttons

  int16_t x, y, h, v;
  ui.bounds(POLY(usb_btn), x, y, h, v);

  default_button_colors();

  cmd.fgcolor(fill_rgb).font(font_medium)
     .enabled(isMediaInserted() && !isPrintingFromMedia())
     .tag(9).button(x, y, h, v,
        isPrintingFromMedia() ?
          F("Printing") :
        #if ENABLED(USB_FLASH_DRIVE_SUPPORT)
          F("USB Drive")
        #else
          F("SD Card")
        #endif
      );

  ui.bounds(POLY(menu_btn), x, y, h, v);
  cmd.tag(10).button(x, y, h, v, F("Menu"));
}

bool StatusScreen::onTouchEnd(uint8_t tag) {
  increment = 3;
  switch(tag) {
    case  9: GOTO_SCREEN(FilesScreen); return true;
    case 10: GOTO_SCREEN(MainMenu);    return true;
    default:                           return false;
  }
}

bool StatusScreen::onTouchHeld(uint8_t tag) {
  if(ExtUI::isMoving()) return false; // Don't allow moves to accumulate
  //constexpr float increment = 1;
  #define UI_INCREMENT_AXIS(axis) MoveAxisScreen::setManualFeedrate(axis, increment); UI_INCREMENT(AxisPosition_mm, axis);
  #define UI_DECREMENT_AXIS(axis) MoveAxisScreen::setManualFeedrate(axis, increment); UI_DECREMENT(AxisPosition_mm, axis);
  switch(tag) {
    case 1: UI_DECREMENT_AXIS(X); break;
    case 2: UI_INCREMENT_AXIS(X); break;
    case 3: UI_DECREMENT_AXIS(Y); break;
    case 4: UI_INCREMENT_AXIS(Y); break;
    case 5: UI_DECREMENT_AXIS(Z); break;
    case 6: UI_INCREMENT_AXIS(Z); break;
    case 7: UI_DECREMENT_AXIS(E0); break;
    case 8: UI_INCREMENT_AXIS(E0); break;
    default: return false;
  }
  #undef UI_DECREMENT_AXIS
  #undef UI_INCREMENT_AXIS
  if(increment < 10)
    increment++;
  return false;
}

#endif // EXTENSIBLE_UI