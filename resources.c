#include <gtk/gtk.h>
#include "resources.h"

static const char *css_data =
"window {"
"   background: #121212;"
"   color: #eeeeee;"
"}"
"label {"
"   color: #eeeeee;"
"   font-size: 18px;"
"}"
"button {"
"   background: #2c2c2c;"
"   color: white;"
"   border-radius: 8px;"
"   padding: 10px;"
"   border: 2px solid #3c3c3c;"
"}"
"button:hover {"
"   background: #444444;"
"}"
".title {"
"   font-size: 32px;"
"   font-weight: bold;"
"}"
".info {"
"   font-size: 20px;"
"   color: #00ffaa;"
"}"
".danger {"
"   font-size: 28px;"
"   color: #ff5555;"
"}"
".copyright {"
"   font-size: 12px;"
"   color: #888888;"
"}"
"window.light {"
"   background: #f0f0f0;"
"   color: #222222;"
"}"
"window.light label {"
"   color: #222222;"
"}"
"window.light button {"
"   background: #ffffff;"
"   color: #222222;"
"   border: 1px solid #cccccc;"
"}"
"window.light button:hover {"
"   background: #e0e0e0;"
"}"
"window.light .title {"
"   color: #111111;"
"}"
"window.light .info {"
"   color: #00aa55;"
"}"
"window.light .danger {"
"   color: #cc0000;"
"}"
"window.light .copyright {"
"   color: #666666;"
"}";

void resources_load_css() {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    g_object_unref(provider);
}
