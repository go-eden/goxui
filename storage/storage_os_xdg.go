// +build !windows,!darwin

package storage

import (
	"errors"
	"github.com/boltdb/bolt"
	"os"
	"path/filepath"
)

// https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html

// Initialize storage variable for linux operation.
func Init(vendor string, appName string) (err error) {
	var globalSettingFolder string
	var cacheFolder string
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
