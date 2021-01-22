#include <src/vnc.h>

#include <glib/gi18n.h>

class VNCIndicator::Impl
{
public:

  Impl():
    m_settings(g_settings_new(m_schema_name)),
    m_action_group(create_action_group())
  {
    // build the vnc icon
    auto icon = g_themed_icon_new_with_default_fallbacks(m_vnc_icon_name);
    auto icon_deleter = [](GIcon* o){g_object_unref(G_OBJECT(o));};
    m_icon.reset(icon, icon_deleter);

    // build the phone profile
    auto menu_model_deleter = [](GMenuModel* o){g_object_unref(G_OBJECT(o));};
    std::shared_ptr<GMenuModel> phone_menu (create_phone_menu(), menu_model_deleter);
    m_phone = std::make_shared<SimpleProfile>("phone", phone_menu);
    update_phone_header();
  }

  ~Impl()
  {
    g_signal_handlers_disconnect_by_data(m_settings, this);
    g_clear_object(&m_action_group);
    g_clear_object(&m_settings);
  }

  GSimpleActionGroup* action_group() const
  {
    return m_action_group;
  }

  std::vector<std::shared_ptr<Profile>> profiles()
  {
    std::vector<std::shared_ptr<Profile>> ret;
    ret.push_back(m_phone);
    return ret;
  }

private:

  /***
  ****  Actions
  ***/

  static gboolean settings_to_action_state(GValue *value,
                                           GVariant *variant,
                                           gpointer /*unused*/)
  {
    g_value_set_variant(value, variant);
    return TRUE;
  }

  static GVariant* action_state_to_settings(const GValue *value,
                                            const GVariantType * /*expected_type*/,
                                            gpointer /*unused*/)
  {
    return g_value_dup_variant(value);
  }
 
  GSimpleActionGroup* create_action_group()
  {
    GSimpleActionGroup* group;
    GSimpleAction* action;

    group = g_simple_action_group_new();
    action = g_simple_action_new_stateful("vnc",
                                          nullptr,
                                          g_variant_new_boolean(false));
    g_settings_bind_with_mapping(m_settings, "vnc",
                                 action, "state",
                                 G_SETTINGS_BIND_DEFAULT,
                                 settings_to_action_state,
                                 action_state_to_settings,
                                 nullptr,
                                 nullptr);
                                 
    g_action_map_add_action(G_ACTION_MAP(group), G_ACTION(action));
    g_object_unref(G_OBJECT(action));
    g_signal_connect_swapped(m_settings, "changed::vnc",
                             G_CALLBACK(on_vnc_setting_changed), this);

    return group;
  }

  /***
  ****  Phone profile
  ***/

  static void on_vnc_setting_changed (gpointer gself)
  {
    static_cast<Impl*>(gself)->update_phone_header();
  }

  GMenuModel* create_phone_menu()
  {
    GMenu* menu;
    GMenuItem* menu_item;

    menu = g_menu_new();

    menu_item = g_menu_item_new(_("VNC"), "indicator.vnc");
    g_menu_item_set_attribute(menu_item, "x-canonical-type", "s", "com.canonical.indicator.switch");
    g_menu_append_item(menu, menu_item);
    g_object_unref(menu_item);

    return G_MENU_MODEL(menu);
  }

  void update_phone_header()
  {
    Header h;
    h.title = _("VNC");
    h.a11y = h.title;
    h.is_visible = g_settings_get_boolean(m_settings, "vnc");
    h.icon = m_icon;
    m_phone->header().set(h);
  }

  /***
  ****
  ***/

  static constexpr char const * m_schema_name {"com.ubuntu.touch.system"};
  static constexpr char const * m_vnc_icon_name {"vnc"};
  GSettings* m_settings = nullptr;
  GSimpleActionGroup* m_action_group = nullptr;
  std::shared_ptr<SimpleProfile> m_phone;
  std::shared_ptr<GIcon> m_icon;
};

/***
****
***/

VNCIndicator::VNCIndicator():
    impl(new Impl())
{
}

VNCIndicator::~VNCIndicator()
{
}

std::vector<std::shared_ptr<Profile>>
VNCIndicator::profiles() const
{
  return impl->profiles();
}

GSimpleActionGroup*
VNCIndicator::action_group() const
{
  return impl->action_group();
}

const char*
VNCIndicator::name() const
{
  return "vnc";
}

/***
****
***/
