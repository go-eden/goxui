#include <stdlib.h>
#include <windows.h>
#include <wininet.h>
#include <stdio.h>

static unsigned long _n_size = sizeof(INTERNET_PER_CONN_OPTION_LIST);

// 执行设置
int _do_set(INTERNET_PER_CONN_OPTION_LIST *list) {
    if (!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, list, _n_size)) {
        return 90;
    }
    if (!InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0)) {
        return 91;
    }
    if (!InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0)) {
        return 92;
    }
    return 0;
}

// 设置Socks代理
int systool_set_proxy_socks(char *addr) {
    INTERNET_PER_CONN_OPTION_LIST list;
    INTERNET_PER_CONN_OPTION option[3];
    option[0].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    option[0].Value.pszValue = (LPSTR) addr;
    option[1].dwOption = INTERNET_PER_CONN_FLAGS;
    option[1].Value.dwValue = PROXY_TYPE_PROXY | PROXY_TYPE_DIRECT;
    option[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    option[2].Value.pszValue = "<local>";
    list.dwSize = _n_size;
    list.pszConnection = NULL;
    list.dwOptionCount = 3;
    list.dwOptionError = 0;
    list.pOptions = option;
    return _do_set(&list);
}

// 设置PAC代理
int systool_set_proxy_pac(char *pacUrl) {
    INTERNET_PER_CONN_OPTION option[2];
    option[0].dwOption = INTERNET_PER_CONN_FLAGS;
    option[0].Value.dwValue = PROXY_TYPE_AUTO_PROXY_URL;
    option[1].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
    option[1].Value.pszValue = pacUrl;
    INTERNET_PER_CONN_OPTION_LIST list;
    list.dwSize = _n_size;
    list.pszConnection = NULL;
    list.dwOptionCount = 2;
    list.dwOptionError = 0;
    list.pOptions = option;
    return _do_set(&list);
}

// 禁用系统代理
int systool_set_proxy_disable() {
    INTERNET_PER_CONN_OPTION option[2];
    option[0].dwOption = INTERNET_PER_CONN_FLAGS;
    option[0].Value.dwValue = PROXY_TYPE_DIRECT;
    option[1].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
    option[1].Value.pszValue = "";
    INTERNET_PER_CONN_OPTION_LIST list;
    list.dwSize = _n_size;
    list.pszConnection = NULL;
    list.dwOptionCount = 2;
    list.dwOptionError = 0;
    list.pOptions = option;
    return _do_set(&list);
}

// 查询Windows系统代理设置
char *systool_get_proxy_pac() {
    unsigned long nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    INTERNET_PER_CONN_OPTION_LIST List;
    INTERNET_PER_CONN_OPTION Option[3];
    Option[0].dwOption = INTERNET_PER_CONN_FLAGS;
    Option[1].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
    Option[2].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    List.pszConnection = NULL;
    List.dwOptionCount = 3;
    List.dwOptionError = 0;
    List.pOptions = Option;
    if (!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize)) {
        printf("InternetQueryOption failed! (%ld)\n", GetLastError());
        return 0;
    }
    DWORD isPac = Option[0].Value.dwValue & PROXY_TYPE_AUTO_PROXY_URL;
    DWORD isSocks = Option[0].Value.dwValue & PROXY_TYPE_PROXY;
    if (isPac != 0) {
        return Option[1].Value.pszValue; // PAC模式
    } else if (isSocks != 0) {
        return Option[2].Value.pszValue; // Socks模式
    }
    return "";
}

int main(int argc, char **argv) {
    if (systool_set_proxy_socks("localhost:8888") != 0) {
        return 99;
    }
    if (systool_set_proxy_pac("http://localhost:9000/test.pac") != 0) {
        return 99;
    }
    if (systool_set_proxy_disable() != 0) {
        return 99;
    }
    printf("TTT: %s", systool_get_proxy_pac());
}
