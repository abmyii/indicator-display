/*
 * Copyright 2014 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *   Charles Kerr <charles.kerr@canonical.com>
 */

#ifndef INDICATOR_DISPLAY_EXPORTER_H
#define INDICATOR_DISPLAY_EXPORTER_H

#include <src/indicator.h>

#include <core/signal.h>

#include <memory>

class Exporter
{
public:
  explicit Exporter(const std::shared_ptr<Indicator>& indicator);
  ~Exporter();
  core::Signal<std::string>& name_lost();

private:
  class Impl;
  std::unique_ptr<Impl> impl;

  Exporter(const Exporter&) =delete;
  Exporter& operator=(const Exporter&) =delete;
};

#endif /* INDICATOR_DISPLAY_EXPORTER_H */
