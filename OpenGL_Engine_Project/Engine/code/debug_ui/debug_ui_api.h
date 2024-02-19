#pragma once
#include "debug_ui_system.h"

#define DBG_UI_REG_LAMBDA(_path_, ...)            \
   {                                              \
      dbg_ui::get_system().registrate_menu(_path_, __VA_ARGS__); \
   }

#define DBG_UI_UNREG(_path_)           \
   {                                   \
      dbg_ui::get_system().unregistrate_menu(_path_); \
   }


#define DBG_UI_SET_ITEM_CHECKED(_path_, _val_)    \
   {                                              \
      dbg_ui::get_system().set_menu_checked(_path_, _val_); \
   }

#define DBG_UI_IS_ITEM_CHECKED(_path_) dbg_ui::get_system().is_item_checked(_path_)
#define DBG_UI_MENU_ITEM_CHECK_LAMBDA(_path_, ...)        \
   {                                                      \
      dbg_ui::get_system().set_check_callback(_path_, __VA_ARGS__); \
   }

#define DBG_UI_REG_LAMBDA_IMPLICIT(_path_, ...)           \
   {                                                      \
      dbg_ui::get_system().register_implicit(_path_, __VA_ARGS__); \
   }
#define DBG_UI_UNREG_IMPLICIT(_path_)          \
   {                                           \
      dbg_ui::get_system().unregister_implicit(_path_); \
   }
