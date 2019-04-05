package storage

import (
	"errors"
	"github.com/boltdb/bolt"
	"os"
	"path/filepath"
)

// Initialize mac's storage variables.
func Init(vendor string, appName string) (err error) {
	home := os.Getenv("HOME")
	// confirm support dir
	var supportDir = home + "/Library/Application Support"
	SupportDir = filepath.Join(supportDir, vendor, appName)
	if err = os.MkdirAll(SupportDir, 0755); err != nil {
		return errors.New("Can't create SupportDir: " + err.Error())
	}

	// confirm cache dir
	var cacheFolder = home + "/Library/Caches"
	CacheDir = filepath.Join(cacheFolder, vendor, appName)
	if err = os.MkdirAll(CacheDir, 0755); err != nil {
		return errors.New("Can't create CacheDir: " + err.Error())
	}

	// prepare storage
	dbFile := filepath.Join(SupportDir, appName+".db")
	if storage, err = bolt.Open(dbFile, 0600, nil); err != nil {
		return errors.New("Can't open db file: " + err.Error())
	}
	return
}
