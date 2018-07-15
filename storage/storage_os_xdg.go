// +build !windows,!darwin

package storage

import (
    "os"
    "errors"
    "github.com/boltdb/bolt"
    "path/filepath"
)

// https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html

// 初始化存储环境, 根据指定的厂商、应用名来创建Support、Cache目录
func Init(vendor string, appName string) (err error) {
    var globalSettingFolder string // 设置目录
    var cacheFolder string         // 缓存目录
    if os.Getenv("XDG_CONFIG_HOME") != "" {
        globalSettingFolder = os.Getenv("XDG_CONFIG_HOME")
    } else {
        globalSettingFolder = filepath.Join(os.Getenv("HOME"), ".config")
    }
    if os.Getenv("XDG_CACHE_HOME") != "" {
        cacheFolder = os.Getenv("XDG_CACHE_HOME")
    } else {
        cacheFolder = filepath.Join(os.Getenv("HOME"), ".cache")
    }
    SupportDir = filepath.Join(globalSettingFolder, vendor, appName)
    CacheDir = filepath.Join(cacheFolder, vendor, appName)
    if err = os.MkdirAll(SupportDir, 0755); err != nil {
        return errors.New("Can't create SupportDir: " + err.Error())
    }
    if err = os.MkdirAll(CacheDir, 0755); err != nil {
        return errors.New("Can't create CacheDir: " + err.Error())
    }
    dbFile := filepath.Join(SupportDir, appName+".db")
    if storage, err = bolt.Open(dbFile, 0600, nil); err != nil {
        return errors.New("Can't open db file: " + err.Error())
    }
    return
}
