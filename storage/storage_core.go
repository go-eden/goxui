// System wide configuration folders:
//
//   - Windows: %PROGRAMDATA% (C:\ProgramData)
//   - Linux/BSDs: ${XDG_CONFIG_DIRS} (/etc/xdg)
//   - MacOSX: "/Library/Application Support"
//
// User wide configuration folders:
//
//   - Windows: %APPDATA% (C:\Users\<User>\AppData\Roaming)
//   - Linux/BSDs: ${XDG_CONFIG_HOME} (${HOME}/.config)
//   - MacOSX: "${HOME}/Library/Application Support"
//
// User wide cache folders:
//
//   - Windows: %LOCALAPPDATA% (C:\Users\<User>\AppData\Local)
//   - Linux/BSDs: ${XDG_CACHE_HOME} (${HOME}/.cache)
//   - MacOSX: "${HOME}/Library/Caches"

package storage

import (
    "github.com/boltdb/bolt"
)

var (
    SupportDir string // 支持文件的磁盘路径
    CacheDir   string // 缓存文件的磁盘路径
    
    storage *bolt.DB    // 全局KV存储表
    bucket  = "default" // 默认Bucket
)

// 查询KEY值
func Get(bucket, key string) (val string, exists bool) {
    if storage == nil {
        panic("storage not init")
    }
    storage.View(func(tx *bolt.Tx) error {
        b := tx.Bucket([]byte(bucket))
        if b == nil {
            return nil
        }
        if data := b.Get([]byte(key)); data != nil {
            exists = true
            val = string(data)
        }
        return nil
    })
    return
}

// 设置KEY值
func Set(bucket, key, val string) {
    if storage == nil {
        panic("storage not init")
    }
    storage.Update(func(tx *bolt.Tx) error {
        tx.CreateBucketIfNotExists([]byte(bucket))
        bucket := tx.Bucket([]byte(bucket))
        if bucket == nil {
            return nil
        }
        return bucket.Put([]byte(key), []byte(val))
    })
}
