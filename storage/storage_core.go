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
	SupportDir string // support file's system path
	CacheDir   string // cache file's system path

	storage *bolt.DB    // Global key-value database
	bucket  = "default" // default bucket's name
)

// Fetch key's value
func Get(bucket, key string) (val string, exists bool) {
	if storage == nil {
		panic("storage not init")
	}
	err := storage.View(func(tx *bolt.Tx) error {
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
	if err != nil {
		panic(err)
	}
	return
}

// Setup key's value
func Set(bucket, key, val string) {
	if storage == nil {
		panic("storage not init")
	}
	err := storage.Update(func(tx *bolt.Tx) error {
		if _, err := tx.CreateBucketIfNotExists([]byte(bucket)); err != nil {
			return err
		}
		bucket := tx.Bucket([]byte(bucket))
		if bucket == nil {
			return nil
		}
		return bucket.Put([]byte(key), []byte(val))
	})
	if err != nil {
		panic(err)
	}
}
