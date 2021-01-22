#ifndef INDICATOR_DISPLAY_VNC_H
#define INDICATOR_DISPLAY_VNC_H

#include <src/indicator.h>

#include <memory> // std::unique_ptr

class VNCIndicator: public Indicator
{
public:
  VNCIndicator();
  ~VNCIndicator();

  const char* name() const;
  GSimpleActionGroup* action_group() const;
  std::vector<std::shared_ptr<Profile>> profiles() const;

protected:
  class Impl;
  std::unique_ptr<Impl> impl;
};

#endif
