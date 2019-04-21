package storage

import (
	"fmt"
	"strconv"
)

func GetInt64(key string, def int64) int64 {
	val, exist := Get(bucket, key)
	if !exist {
		return def
	}
	i, err := strconv.ParseInt(val, 10, 64)
	if err != nil {
		return def
	}
	return i
}

func SetInt64(key string, val int64) {
	Set(bucket, key, fmt.Sprintf("%d", val))
}

func GetFloat64(key string, def float64) float64 {
	val, exist := Get(bucket, key)
	if !exist {
		return def
	}
	i, err := strconv.ParseFloat(val, 64)
	if err != nil {
		return def
	}
	return i
}

func SetFloat64(key string, val float64) {
	Set(bucket, key, fmt.Sprintf("%f", val))
}

func SetString(key string, val string) {
	Set(bucket, key, val)
}

func GetString(key string, def string) string {
	val, exist := Get(bucket, key)
	if !exist {
		return def
	}
	return val
}
