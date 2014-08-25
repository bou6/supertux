//  SuperTux
//  Copyright (C) 2014 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "gui/dialog.hpp"

#include "control/controller.hpp"
#include "gui/menu_manager.hpp"
#include "supertux/resources.hpp"
#include "video/drawing_context.hpp"

Dialog::Dialog() :
  m_text(),
  m_buttons(),
  m_selected_button()
{
}

Dialog::~Dialog()
{
}

void
Dialog::set_text(const std::string& text)
{
  m_text = text;
}

void
Dialog::add_button(const std::string& text)
{
  m_buttons.push_back(text);
}

void
Dialog::process_input(const Controller& controller)
{
  if (controller.pressed(Controller::LEFT))
  {
    m_selected_button -= 1;
    m_selected_button = std::max(m_selected_button, 0);
  }

  if (controller.pressed(Controller::RIGHT))
  {
    m_selected_button += 1;
    m_selected_button = std::min(m_selected_button, static_cast<int>(m_buttons.size()) - 1);
  }

  if (controller.pressed(Controller::ACTION) ||
      controller.pressed(Controller::MENU_SELECT))
  {
    on_select(m_selected_button);

    // warning: this will "delete this"
    MenuManager::instance().set_dialog({});
  }
}

void
Dialog::draw(DrawingContext& ctx)
{
  Vector center(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

  Sizef text_size(Resources::normal_font->get_text_width(m_text),
                  Resources::normal_font->get_text_height(m_text));

  Rectf text_rect(Vector(center.x - text_size.width/2,
                         center.y - text_size.height/2),
                  text_size);

  Rectf bg_rect = text_rect;
  bg_rect.p2.y += 44;

  // draw background rect
  ctx.draw_filled_rect(bg_rect.grown(12.0f),
                       Color(0.2f, 0.3f, 0.4f, 0.8f),
                       16.0f,
                       LAYER_GUI-10);

  ctx.draw_filled_rect(bg_rect.grown(8.0f),
                       Color(0.6f, 0.7f, 0.8f, 0.5f),
                       16.0f,
                       LAYER_GUI-10);

  // draw text
  ctx.draw_text(Resources::normal_font, m_text,
                Vector(bg_rect.p1.x + bg_rect.get_width()/2.0f,
                       bg_rect.p1.y),
                ALIGN_CENTER, LAYER_GUI);

  // draw HL line
  ctx.draw_filled_rect(Vector(bg_rect.p1.x, bg_rect.p2.y - 35),
                           Vector(bg_rect.get_width(), 4),
                           Color(0.6f, 0.7f, 1.0f, 1.0f), LAYER_GUI);
  ctx.draw_filled_rect(Vector(bg_rect.p1.x, bg_rect.p2.y - 35),
                           Vector(bg_rect.get_width(), 2),
                           Color(1.0f, 1.0f, 1.0f, 1.0f), LAYER_GUI);

  // draw buttons
  for(int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
  {
    float segment_width = bg_rect.get_width() / m_buttons.size();
    float button_width = segment_width * 0.95;
    float button_height = 24.0f;
    Vector pos(bg_rect.p1.x + segment_width/2.0f + i * segment_width,
               bg_rect.p2.y - 12);

    if (i == m_selected_button)
    {
      float blink = (sinf(real_time * M_PI * 1.0f)/2.0f + 0.5f) * 0.5f + 0.25f;
      ctx.draw_filled_rect(Rectf(Vector(pos.x - button_width/2, pos.y - button_height/2),
                                 Vector(pos.x + button_width/2, pos.y + button_height/2)).grown(2.0f),
                           Color(1.0f, 1.0f, 1.0f, blink),
                           14.0f,
                           LAYER_GUI-10);
      ctx.draw_filled_rect(Rectf(Vector(pos.x - button_width/2, pos.y - button_height/2),
                                 Vector(pos.x + button_width/2, pos.y + button_height/2)),
                           Color(1.0f, 1.0f, 1.0f, 0.5f),
                           12.0f,
                           LAYER_GUI-10);
    }

    ctx.draw_text(Resources::normal_font, m_buttons[i],
                  Vector(pos.x, pos.y - int(Resources::normal_font->get_height()/2)),
                  ALIGN_CENTER, LAYER_GUI);
  }
}

void
Dialog::on_select(int id)
{
}

/* EOF */
