#include <gio/gio.h>
#include <string.h>
#include <stdio.h>

// 禁用代理
int systool_set_proxy_disable() {
    int ret = 0;
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    g_type_init(); // deprecated since version 2.36, must leave here or prior glib will crash
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
    GSettings *setting = g_settings_new("org.gnome.system.proxy");

    gboolean success = g_settings_set_string(setting, "mode", "none");
    if (!success) {
        ret = 99;
        printf("set proxy mode failed. \n");
        goto defer;
    }

    defer:
    g_settings_sync();
    g_object_unref(setting);
    return ret;
}

// 设置PAC代理
int systool_set_proxy_pac(char *addr) {
    int ret = 0;
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    g_type_init(); // deprecated since version 2.36, must leave here or prior glib will crash
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
    GSettings *setting = g_settings_new("org.gnome.system.proxy");

    gboolean success = g_settings_set_string(setting, "mode", "auto");
    if (!success) {
        ret = 99;
        printf("set proxy mode failed. \n");
        goto defer;
    }
    success = g_settings_set_string(setting, "autoconfig-url", addr);
    if (!success) {
        ret = 99;
        printf("set proxy pac failed. \n");
        goto defer;
    }

    defer:
    g_settings_sync();
    g_object_unref(setting);
    return ret;
}

// 设置Socks5代理
int systool_set_proxy_socks(char *host, int port) {
    int ret = 0;
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    g_type_init(); // deprecated since version 2.36, must leave here or prior glib will crash
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
    GSettings *setting = g_settings_new("org.gnome.system.proxy");
    GSettings *sockSetting = g_settings_new("org.gnome.system.proxy.socks");

    gboolean success = g_settings_set_string(setting, "mode", "manual");
    if (!success) {
        ret = 99;
        printf("set proxy mode failed. \n");
        goto defer;
    }
    success = g_settings_set_string(sockSetting, "host", host);
    if (!success) {
        ret = 99;
        printf("set proxy socks host failed. \n");
        goto defer;
    }
    success = g_settings_set_int(sockSetting, "port", port);
    if (!success) {
        ret = 99;
        printf("set proxy socks port failed. \n");
        goto defer;
    }

    defer:
    g_settings_sync();
    g_object_unref(setting);
    g_object_unref(sockSetting);
    return ret;
}

// 查询系统代理
char *systool_get_proxy() {
    char *ret = "unknown";
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    g_type_init(); // deprecated since version 2.36, must leave here or prior glib will crash
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
    GSettings *setting = g_settings_new("org.gnome.system.proxy");

    char *mode = g_settings_get_string(setting, "mode");
    if (strcmp(mode, "none") == 0) {
        ret = "";
    }
    if (strcmp(mode, "auto") == 0) {
        char *addr = g_settings_get_string(setting, "autoconfig-url");
        if (addr != 0) {
            ret = addr;
        }
    }
    if (strcmp(mode, "manual") == 0) {
        static char buf[96]; // not thread safe
        GSettings *sockSetting = g_settings_new("org.gnome.system.proxy.socks");
        char *host = g_settings_get_string(sockSetting, "host");
        int port = g_settings_get_int(sockSetting, "port");
        if (host != 0) {
            sprintf(buf, "%s:%d", host, port);
            ret = buf;
        }
        g_object_unref(sockSetting);
    }
    g_object_unref(setting);
    return ret;
}

int main(int argc, char *argv[]) {
    printf("PROXY: %s \n", systool_get_proxy());
    systool_set_proxy_disable();
    printf("PROXY: %s \n", systool_get_proxy());
    systool_set_proxy_pac("http://localhost:9090/ddd.pac");
    printf("PROXY: %s \n", systool_get_proxy());
    systool_set_proxy_socks("localhost", 8009);
    printf("PROXY: %s \n", systool_get_proxy());
    systool_set_proxy_disable();
    printf("PROXY: %s \n", systool_get_proxy());
    return 0;
}