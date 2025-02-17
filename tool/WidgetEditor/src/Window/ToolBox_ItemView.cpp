﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "ToolBox.hpp"

#include "ToolBox_Item.hpp"
#include "ToolBox_Drawing.hpp"
#include "ToolBox_ItemView.hpp"
#include "ToolBox_ControlWindow.hpp"

unsigned char bitmap_expand_png[558] = {
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
	0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF, 0x61, 0x00, 0x00, 0x01,
	0x84, 0x69, 0x43, 0x43, 0x50, 0x49, 0x43, 0x43, 0x20, 0x70, 0x72, 0x6F,
	0x66, 0x69, 0x6C, 0x65, 0x00, 0x00, 0x28, 0x91, 0x7D, 0x91, 0x3D, 0x48,
	0xC3, 0x50, 0x14, 0x85, 0x4F, 0x53, 0xB5, 0x45, 0x2A, 0x22, 0x76, 0x10,
	0x71, 0xC8, 0x50, 0x9D, 0xEC, 0xA2, 0x22, 0x8E, 0xA5, 0x8A, 0x45, 0xB0,
	0x50, 0xDA, 0x0A, 0xAD, 0x3A, 0x98, 0xBC, 0xF4, 0x0F, 0x9A, 0x34, 0x24,
	0x29, 0x2E, 0x8E, 0x82, 0x6B, 0xC1, 0xC1, 0x9F, 0xC5, 0xAA, 0x83, 0x8B,
	0xB3, 0xAE, 0x0E, 0xAE, 0x82, 0x20, 0xF8, 0x03, 0xE2, 0x2E, 0x38, 0x29,
	0xBA, 0x48, 0x89, 0xF7, 0x25, 0x85, 0x16, 0x31, 0x5E, 0x78, 0xBC, 0x8F,
	0xF3, 0xEE, 0x39, 0xBC, 0x77, 0x1F, 0x20, 0x34, 0xAB, 0x4C, 0x35, 0x7B,
	0x62, 0x80, 0xAA, 0x59, 0x46, 0x3A, 0x11, 0x17, 0x73, 0xF9, 0x55, 0x31,
	0xF0, 0x0A, 0x01, 0x43, 0xF0, 0xA1, 0x0F, 0x41, 0x89, 0x99, 0x7A, 0x32,
	0xB3, 0x98, 0x85, 0x67, 0x7D, 0xDD, 0x53, 0x27, 0xD5, 0x5D, 0x94, 0x67,
	0x79, 0xF7, 0xFD, 0x59, 0x03, 0x4A, 0xC1, 0x64, 0x80, 0x4F, 0x24, 0x8E,
	0x31, 0xDD, 0xB0, 0x88, 0x37, 0x88, 0x67, 0x37, 0x2D, 0x9D, 0xF3, 0x3E,
	0x71, 0x98, 0x95, 0x25, 0x85, 0xF8, 0x9C, 0x78, 0xD2, 0xA0, 0x0B, 0x12,
	0x3F, 0x72, 0x5D, 0x76, 0xF9, 0x8D, 0x73, 0xC9, 0x61, 0x81, 0x67, 0x86,
	0x8D, 0x6C, 0x7A, 0x9E, 0x38, 0x4C, 0x2C, 0x96, 0xBA, 0x58, 0xEE, 0x62,
	0x56, 0x36, 0x54, 0xE2, 0x19, 0xE2, 0x88, 0xA2, 0x6A, 0x94, 0x2F, 0xE4,
	0x5C, 0x56, 0x38, 0x6F, 0x71, 0x56, 0xAB, 0x75, 0xD6, 0xBE, 0x27, 0x7F,
	0x61, 0xA8, 0xA0, 0xAD, 0x64, 0xB8, 0x4E, 0x6B, 0x0C, 0x09, 0x2C, 0x21,
	0x89, 0x14, 0x44, 0xC8, 0xA8, 0xA3, 0x82, 0x2A, 0x2C, 0x44, 0x69, 0xD7,
	0x48, 0x31, 0x91, 0xA6, 0xF3, 0xB8, 0x87, 0x7F, 0xD4, 0xF1, 0xA7, 0xC8,
	0x25, 0x93, 0xAB, 0x02, 0x46, 0x8E, 0x05, 0xD4, 0xA0, 0x42, 0x72, 0xFC,
	0xE0, 0x7F, 0xF0, 0x7B, 0xB6, 0x66, 0x71, 0x7A, 0xCA, 0x4D, 0x0A, 0xC5,
	0x81, 0xDE, 0x17, 0xDB, 0xFE, 0x18, 0x07, 0x02, 0xBB, 0x40, 0xAB, 0x61,
	0xDB, 0xDF, 0xC7, 0xB6, 0xDD, 0x3A, 0x01, 0xFC, 0xCF, 0xC0, 0x95, 0xD6,
	0xF1, 0xD7, 0x9A, 0xC0, 0xDC, 0x27, 0xE9, 0x8D, 0x8E, 0x16, 0x39, 0x02,
	0x06, 0xB7, 0x81, 0x8B, 0xEB, 0x8E, 0x26, 0xEF, 0x01, 0x97, 0x3B, 0xC0,
	0xC8, 0x93, 0x2E, 0x19, 0x92, 0x23, 0xF9, 0x69, 0x09, 0xC5, 0x22, 0xF0,
	0x7E, 0x46, 0xDF, 0x94, 0x07, 0x86, 0x6F, 0x81, 0xFE, 0x35, 0x77, 0x6E,
	0xED, 0x73, 0x9C, 0x3E, 0x00, 0x59, 0x9A, 0xD5, 0xF2, 0x0D, 0x70, 0x70,
	0x08, 0x4C, 0x94, 0x28, 0x7B, 0xDD, 0xE3, 0xDD, 0xC1, 0xEE, 0xB9, 0xFD,
	0xDB, 0xD3, 0x9E, 0xDF, 0x0F, 0x15, 0x63, 0x72, 0x81, 0x36, 0x08, 0xB1,
	0xC4, 0x00, 0x00, 0x00, 0x06, 0x62, 0x4B, 0x47, 0x44, 0x00, 0xF8, 0x00,
	0xFF, 0x00, 0x35, 0x69, 0x2C, 0x50, 0x2D, 0x00, 0x00, 0x00, 0x09, 0x70,
	0x48, 0x59, 0x73, 0x00, 0x00, 0x0E, 0xC0, 0x00, 0x00, 0x0E, 0xC0, 0x01,
	0x6A, 0xD6, 0x89, 0x09, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4D, 0x45,
	0x07, 0xE9, 0x02, 0x11, 0x01, 0x0A, 0x2D, 0xD1, 0x08, 0x5D, 0xB8, 0x00,
	0x00, 0x00, 0x2B, 0x49, 0x44, 0x41, 0x54, 0x38, 0xCB, 0x63, 0x60, 0x18,
	0x05, 0xC4, 0x80, 0xFF, 0xF8, 0x24, 0x99, 0xA8, 0x61, 0x08, 0xB1, 0x9A,
	0xFF, 0x53, 0xC3, 0x0B, 0xFF, 0xA9, 0x11, 0x06, 0xFF, 0x07, 0xCC, 0x05,
	0x14, 0x85, 0xC1, 0x7F, 0x9A, 0xA5, 0x83, 0x61, 0x00, 0x00, 0x48, 0xE2,
	0x0E, 0xF4, 0xA9, 0xA0, 0x8C, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45,
	0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

unsigned char bitmap_collapse_png[557] = {
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
	0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF, 0x61, 0x00, 0x00, 0x01,
	0x84, 0x69, 0x43, 0x43, 0x50, 0x49, 0x43, 0x43, 0x20, 0x70, 0x72, 0x6F,
	0x66, 0x69, 0x6C, 0x65, 0x00, 0x00, 0x28, 0x91, 0x7D, 0x91, 0x3D, 0x48,
	0xC3, 0x50, 0x14, 0x85, 0x4F, 0x53, 0xB5, 0x45, 0x2A, 0x22, 0x76, 0x10,
	0x71, 0xC8, 0x50, 0x9D, 0xEC, 0xA2, 0x22, 0x8E, 0xA5, 0x8A, 0x45, 0xB0,
	0x50, 0xDA, 0x0A, 0xAD, 0x3A, 0x98, 0xBC, 0xF4, 0x0F, 0x9A, 0x34, 0x24,
	0x29, 0x2E, 0x8E, 0x82, 0x6B, 0xC1, 0xC1, 0x9F, 0xC5, 0xAA, 0x83, 0x8B,
	0xB3, 0xAE, 0x0E, 0xAE, 0x82, 0x20, 0xF8, 0x03, 0xE2, 0x2E, 0x38, 0x29,
	0xBA, 0x48, 0x89, 0xF7, 0x25, 0x85, 0x16, 0x31, 0x5E, 0x78, 0xBC, 0x8F,
	0xF3, 0xEE, 0x39, 0xBC, 0x77, 0x1F, 0x20, 0x34, 0xAB, 0x4C, 0x35, 0x7B,
	0x62, 0x80, 0xAA, 0x59, 0x46, 0x3A, 0x11, 0x17, 0x73, 0xF9, 0x55, 0x31,
	0xF0, 0x0A, 0x01, 0x43, 0xF0, 0xA1, 0x0F, 0x41, 0x89, 0x99, 0x7A, 0x32,
	0xB3, 0x98, 0x85, 0x67, 0x7D, 0xDD, 0x53, 0x27, 0xD5, 0x5D, 0x94, 0x67,
	0x79, 0xF7, 0xFD, 0x59, 0x03, 0x4A, 0xC1, 0x64, 0x80, 0x4F, 0x24, 0x8E,
	0x31, 0xDD, 0xB0, 0x88, 0x37, 0x88, 0x67, 0x37, 0x2D, 0x9D, 0xF3, 0x3E,
	0x71, 0x98, 0x95, 0x25, 0x85, 0xF8, 0x9C, 0x78, 0xD2, 0xA0, 0x0B, 0x12,
	0x3F, 0x72, 0x5D, 0x76, 0xF9, 0x8D, 0x73, 0xC9, 0x61, 0x81, 0x67, 0x86,
	0x8D, 0x6C, 0x7A, 0x9E, 0x38, 0x4C, 0x2C, 0x96, 0xBA, 0x58, 0xEE, 0x62,
	0x56, 0x36, 0x54, 0xE2, 0x19, 0xE2, 0x88, 0xA2, 0x6A, 0x94, 0x2F, 0xE4,
	0x5C, 0x56, 0x38, 0x6F, 0x71, 0x56, 0xAB, 0x75, 0xD6, 0xBE, 0x27, 0x7F,
	0x61, 0xA8, 0xA0, 0xAD, 0x64, 0xB8, 0x4E, 0x6B, 0x0C, 0x09, 0x2C, 0x21,
	0x89, 0x14, 0x44, 0xC8, 0xA8, 0xA3, 0x82, 0x2A, 0x2C, 0x44, 0x69, 0xD7,
	0x48, 0x31, 0x91, 0xA6, 0xF3, 0xB8, 0x87, 0x7F, 0xD4, 0xF1, 0xA7, 0xC8,
	0x25, 0x93, 0xAB, 0x02, 0x46, 0x8E, 0x05, 0xD4, 0xA0, 0x42, 0x72, 0xFC,
	0xE0, 0x7F, 0xF0, 0x7B, 0xB6, 0x66, 0x71, 0x7A, 0xCA, 0x4D, 0x0A, 0xC5,
	0x81, 0xDE, 0x17, 0xDB, 0xFE, 0x18, 0x07, 0x02, 0xBB, 0x40, 0xAB, 0x61,
	0xDB, 0xDF, 0xC7, 0xB6, 0xDD, 0x3A, 0x01, 0xFC, 0xCF, 0xC0, 0x95, 0xD6,
	0xF1, 0xD7, 0x9A, 0xC0, 0xDC, 0x27, 0xE9, 0x8D, 0x8E, 0x16, 0x39, 0x02,
	0x06, 0xB7, 0x81, 0x8B, 0xEB, 0x8E, 0x26, 0xEF, 0x01, 0x97, 0x3B, 0xC0,
	0xC8, 0x93, 0x2E, 0x19, 0x92, 0x23, 0xF9, 0x69, 0x09, 0xC5, 0x22, 0xF0,
	0x7E, 0x46, 0xDF, 0x94, 0x07, 0x86, 0x6F, 0x81, 0xFE, 0x35, 0x77, 0x6E,
	0xED, 0x73, 0x9C, 0x3E, 0x00, 0x59, 0x9A, 0xD5, 0xF2, 0x0D, 0x70, 0x70,
	0x08, 0x4C, 0x94, 0x28, 0x7B, 0xDD, 0xE3, 0xDD, 0xC1, 0xEE, 0xB9, 0xFD,
	0xDB, 0xD3, 0x9E, 0xDF, 0x0F, 0x15, 0x63, 0x72, 0x81, 0x36, 0x08, 0xB1,
	0xC4, 0x00, 0x00, 0x00, 0x06, 0x62, 0x4B, 0x47, 0x44, 0x00, 0xF8, 0x00,
	0xFF, 0x00, 0x35, 0x69, 0x2C, 0x50, 0x2D, 0x00, 0x00, 0x00, 0x09, 0x70,
	0x48, 0x59, 0x73, 0x00, 0x00, 0x0E, 0xC0, 0x00, 0x00, 0x0E, 0xC0, 0x01,
	0x6A, 0xD6, 0x89, 0x09, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4D, 0x45,
	0x07, 0xE9, 0x02, 0x11, 0x01, 0x0C, 0x2B, 0x6E, 0x31, 0x5F, 0x0B, 0x00,
	0x00, 0x00, 0x2A, 0x49, 0x44, 0x41, 0x54, 0x38, 0xCB, 0x63, 0x60, 0x18,
	0x05, 0x03, 0x02, 0xFE, 0x23, 0x73, 0x98, 0x28, 0xD1, 0x4C, 0xAA, 0x01,
	0xFF, 0xB1, 0x09, 0x32, 0x51, 0xA2, 0x99, 0x58, 0x03, 0xFE, 0xE3, 0x93,
	0x64, 0xA2, 0x44, 0xF3, 0x28, 0xA0, 0x12, 0x00, 0x00, 0x04, 0x1D, 0x06,
	0x05, 0x09, 0x83, 0x01, 0x53, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E,
	0x44, 0xAE, 0x42, 0x60, 0x82
};

unsigned char bitmap_item_png[712] = {
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
	0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF, 0x61, 0x00, 0x00, 0x01,
	0x83, 0x69, 0x43, 0x43, 0x50, 0x49, 0x43, 0x43, 0x20, 0x70, 0x72, 0x6F,
	0x66, 0x69, 0x6C, 0x65, 0x00, 0x00, 0x28, 0x91, 0x7D, 0x91, 0x3D, 0x48,
	0xC3, 0x40, 0x1C, 0xC5, 0x5F, 0x53, 0x4B, 0x45, 0x2A, 0x22, 0x56, 0x10,
	0x71, 0xC8, 0x50, 0x9D, 0xEC, 0xA2, 0x22, 0x8E, 0xA5, 0x8A, 0x45, 0xB0,
	0x50, 0xDA, 0x0A, 0xAD, 0x3A, 0x98, 0x5C, 0xFA, 0x05, 0x4D, 0x1A, 0x92,
	0x14, 0x17, 0x47, 0xC1, 0xB5, 0xE0, 0xE0, 0xC7, 0x62, 0xD5, 0xC1, 0xC5,
	0x59, 0x57, 0x07, 0x57, 0x41, 0x10, 0xFC, 0x00, 0x71, 0x17, 0x9C, 0x14,
	0x5D, 0xA4, 0xC4, 0xFF, 0x25, 0x85, 0x16, 0x31, 0x1E, 0x1C, 0xF7, 0xE3,
	0xDD, 0xBD, 0xC7, 0xDD, 0x3B, 0x40, 0x68, 0x56, 0x99, 0x6A, 0xF6, 0xC4,
	0x00, 0x55, 0xB3, 0x8C, 0x74, 0x22, 0x2E, 0xE6, 0xF2, 0xAB, 0x62, 0xF0,
	0x15, 0x02, 0x06, 0x11, 0xC0, 0x30, 0xFC, 0x12, 0x33, 0xF5, 0x64, 0x66,
	0x31, 0x0B, 0xCF, 0xF1, 0x75, 0x0F, 0x1F, 0x5F, 0xEF, 0xA2, 0x3C, 0xCB,
	0xFB, 0xDC, 0x9F, 0xA3, 0x5F, 0x29, 0x98, 0x0C, 0xF0, 0x89, 0xC4, 0x31,
	0xA6, 0x1B, 0x16, 0xF1, 0x06, 0xF1, 0xEC, 0xA6, 0xA5, 0x73, 0xDE, 0x27,
	0x0E, 0xB3, 0xB2, 0xA4, 0x10, 0x9F, 0x13, 0x4F, 0x1A, 0x74, 0x41, 0xE2,
	0x47, 0xAE, 0xCB, 0x2E, 0xBF, 0x71, 0x2E, 0x39, 0x2C, 0xF0, 0xCC, 0xB0,
	0x91, 0x4D, 0xCF, 0x13, 0x87, 0x89, 0xC5, 0x52, 0x17, 0xCB, 0x5D, 0xCC,
	0xCA, 0x86, 0x4A, 0x3C, 0x43, 0x1C, 0x51, 0x54, 0x8D, 0xF2, 0x85, 0x9C,
	0xCB, 0x0A, 0xE7, 0x2D, 0xCE, 0x6A, 0xB5, 0xCE, 0xDA, 0xF7, 0xE4, 0x2F,
	0x0C, 0x15, 0xB4, 0x95, 0x0C, 0xD7, 0x69, 0x8E, 0x21, 0x81, 0x25, 0x24,
	0x91, 0x82, 0x08, 0x19, 0x75, 0x54, 0x50, 0x85, 0x85, 0x28, 0xAD, 0x1A,
	0x29, 0x26, 0xD2, 0xB4, 0x1F, 0xF7, 0xF0, 0x8F, 0x3A, 0xFE, 0x14, 0xB9,
	0x64, 0x72, 0x55, 0xC0, 0xC8, 0xB1, 0x80, 0x1A, 0x54, 0x48, 0x8E, 0x1F,
	0xFC, 0x0F, 0x7E, 0x77, 0x6B, 0x16, 0xA7, 0xA7, 0xDC, 0xA4, 0x50, 0x1C,
	0x08, 0xBC, 0xD8, 0xF6, 0xC7, 0x38, 0x10, 0xDC, 0x05, 0x5A, 0x0D, 0xDB,
	0xFE, 0x3E, 0xB6, 0xED, 0xD6, 0x09, 0xE0, 0x7F, 0x06, 0xAE, 0xB4, 0x8E,
	0xBF, 0xD6, 0x04, 0xE6, 0x3E, 0x49, 0x6F, 0x74, 0xB4, 0xC8, 0x11, 0x30,
	0xB0, 0x0D, 0x5C, 0x5C, 0x77, 0x34, 0x79, 0x0F, 0xB8, 0xDC, 0x01, 0x46,
	0x9E, 0x74, 0xC9, 0x90, 0x1C, 0xC9, 0x4F, 0x53, 0x28, 0x16, 0x81, 0xF7,
	0x33, 0xFA, 0xA6, 0x3C, 0x30, 0x74, 0x0B, 0xF4, 0xAD, 0xB9, 0xBD, 0xB5,
	0xF7, 0x71, 0xFA, 0x00, 0x64, 0xA9, 0xAB, 0xE5, 0x1B, 0xE0, 0xE0, 0x10,
	0x98, 0x28, 0x51, 0xF6, 0xBA, 0xC7, 0xBB, 0x7B, 0xBB, 0x7B, 0xFB, 0xF7,
	0x4C, 0xBB, 0xBF, 0x1F, 0x38, 0x67, 0x72, 0x8F, 0xEB, 0x43, 0xBA, 0x5D,
	0x00, 0x00, 0x00, 0x06, 0x62, 0x4B, 0x47, 0x44, 0x00, 0xF8, 0x00, 0xFF,
	0x00, 0x35, 0x69, 0x2C, 0x50, 0x2D, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48,
	0x59, 0x73, 0x00, 0x00, 0x0E, 0xC0, 0x00, 0x00, 0x0E, 0xC0, 0x01, 0x6A,
	0xD6, 0x89, 0x09, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4D, 0x45, 0x07,
	0xE9, 0x02, 0x11, 0x05, 0x1B, 0x34, 0xE1, 0xB3, 0x7E, 0xB4, 0x00, 0x00,
	0x00, 0xC6, 0x49, 0x44, 0x41, 0x54, 0x38, 0xCB, 0xB5, 0xD3, 0xB1, 0x4A,
	0x03, 0x51, 0x10, 0x05, 0xD0, 0xB3, 0xC1, 0x22, 0x7F, 0x12, 0x44, 0x2C,
	0x82, 0xA6, 0x49, 0xE1, 0x47, 0xA4, 0x14, 0x82, 0x5A, 0x8A, 0xFE, 0x90,
	0xBD, 0x9D, 0x95, 0x58, 0xA7, 0x11, 0x91, 0x2C, 0x84, 0x80, 0x55, 0x52,
	0x04, 0xBF, 0xC2, 0x22, 0x26, 0x2C, 0xB1, 0x19, 0xE5, 0xF1, 0x10, 0x61,
	0x37, 0xEB, 0xC0, 0x14, 0x77, 0x86, 0x79, 0x73, 0xE7, 0xCE, 0xBC, 0x02,
	0x53, 0x9C, 0x68, 0x66, 0xF3, 0x02, 0x5B, 0x6C, 0xB0, 0xA8, 0x59, 0x7C,
	0x88, 0xAE, 0x78, 0x60, 0xD6, 0xA0, 0xFB, 0x0C, 0xD5, 0x41, 0x16, 0x3C,
	0xC6, 0x19, 0x26, 0x58, 0x62, 0x88, 0x7E, 0x92, 0xAF, 0x70, 0x8F, 0x8F,
	0xB4, 0x28, 0x65, 0x70, 0x83, 0x1D, 0x2E, 0x02, 0xBF, 0x05, 0x4E, 0xFD,
	0xEA, 0x2F, 0x06, 0x13, 0x5C, 0xE2, 0x25, 0xF0, 0x39, 0x4E, 0x93, 0xFC,
	0x1A, 0x8F, 0xF9, 0x2C, 0x7B, 0x69, 0xD0, 0xC9, 0x82, 0x63, 0xAC, 0x30,
	0x0A, 0x7C, 0x17, 0xF8, 0xDB, 0x17, 0xA1, 0xCB, 0x8F, 0x75, 0x6A, 0x76,
	0x2D, 0x42, 0x87, 0xF6, 0x46, 0xC8, 0x45, 0xEC, 0x61, 0x80, 0x57, 0xBC,
	0xE3, 0x28, 0x5B, 0xE3, 0x27, 0x9E, 0x42, 0xCC, 0xFF, 0x59, 0xE3, 0x33,
	0x6E, 0x51, 0x06, 0xBE, 0xFE, 0xE5, 0x90, 0x1E, 0x5A, 0xD5, 0xA0, 0x95,
	0xCF, 0x54, 0x06, 0xB5, 0x26, 0x5E, 0x7E, 0x01, 0xA4, 0x2C, 0x53, 0xAB,
	0xA7, 0xD7, 0xB9, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44,
	0xAE, 0x42, 0x60, 0x82
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ToolBox::ItemView::ItemView(ToolBox::ControlWindow* window) :
	_Window(window)
{
	//-----------------------------------------------------------------------
	getWindow()->getViewport()->setWindowSize(0, 0);
	getWindow()->getViewport()->setDocumentSize(0, 0);
	getWindow()->getViewport()->enableVScrollbar(true);


	//-----------------------------------------------------------------------
	_ItemViewDrawing = std::make_shared<ToolBox::ItemViewDrawing>();


	//-----------------------------------------------------------------------
	ToolBox::ItemDrawingSharedPtr itemDrawing;
	

	_GroupItemDrawing = std::make_shared<ToolBox::GroupItemDrawing>();
	_ItemDrawings.push_back(_GroupItemDrawing);
	_SubItemDrawing = std::make_shared<ToolBox::SubItemDrawing>();
	_ItemDrawings.push_back(_SubItemDrawing);


	//-----------------------------------------------------------------------
#if 0
	ToolBox::GroupItemSharedPtr groupItem;
	ToolBox::GroupItemSharedPtr subGroupItem;
	ToolBox::SubItemSharedPtr subItem;

	groupItem = makeGroupItem(makeID(), L"GroupItem0", L"collapse.png");
	addItem(nullptr, groupItem);

	groupItem = makeGroupItem(makeID(), L"GroupItem1", L"expand.png");
	addItem(nullptr, groupItem);

	subItem = makeSubItem(makeID(), L"SubItem0", L"item.png");
	addItem(nullptr, subItem);

	subItem = makeSubItem(makeID(), L"SubItem1", L"item.png");
	addItem(nullptr, subItem);

	subItem = makeSubItem(makeID(), L"SubItem2", L"item.png");
	addItem(nullptr, subItem);

	subItem = makeSubItem(makeID(), L"SubItem3", L"item.png");
	addItem(nullptr, subItem);

	subItem = makeSubItem(makeID(), L"SubItem4");
	addItem(nullptr, subItem);

	subItem = makeSubItem(makeID(), L"SubItem5");
	addItem(groupItem, subItem);

	subGroupItem = makeGroupItem(makeID(), L"GroupItem1", L"expand.png");
	addItem(groupItem, subGroupItem);
	subItem = makeSubItem(makeID(), L"SubItem6", L"item.png");
	addItem(subGroupItem, subItem);
	subItem = makeSubItem(makeID(), L"SubItem7", L"item.png");
	addItem(subGroupItem, subItem);
	subItem = makeSubItem(makeID(), L"SubItem8", L"item.png");
	addItem(subGroupItem, subItem);
#else
	ToolBox::GroupItemSharedPtr rootGroupItem;
	ToolBox::GroupItemSharedPtr groupItem;
	ToolBox::GroupItemSharedPtr subGroupItem;
	ToolBox::SubItemSharedPtr subItem;

	rootGroupItem = makeGroupItem(makeID(), L"명령", L"expand.png");
	addItem(nullptr, rootGroupItem);

	groupItem = makeGroupItem(makeID(), L"편집", L"expand.png");
	addItem(rootGroupItem, groupItem);

	subItem = makeSubItem(makeID(), L"실행 취소", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"다시 실행", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"잘라내기", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"복사", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"붙여넣기", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"삭제", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"선택 취소", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"전체 선택", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	groupItem = makeGroupItem(makeID(), L"이동", L"expand.png");
	addItem(rootGroupItem, groupItem);

	subItem = makeSubItem(makeID(), L"앞으로", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"뒤로", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"맨 앞으로", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"맨 뒤로", std::wstring(), ToolBox::ItemStyle::Button);
	addItem(groupItem, subItem);

	groupItem = makeGroupItem(makeID(), L"위젯", L"expand.png");
	addItem(nullptr, groupItem);

	subItem = makeSubItem(makeID(), L"선", L"item.png");
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"네모", L"item.png");
	addItem(groupItem, subItem);

	subItem = makeSubItem(makeID(), L"원", L"item.png");
	addItem(groupItem, subItem);
#endif


	_BitmapList.addBitmap(L"expand.png", cx::gw::makeBitmap(bitmap_expand_png, sizeof(bitmap_expand_png)));
	_BitmapList.addBitmap(L"collapse.png", cx::gw::makeBitmap(bitmap_collapse_png, sizeof(bitmap_collapse_png)));
	_BitmapList.addBitmap(L"item.png", cx::gw::makeBitmap(bitmap_item_png, sizeof(bitmap_item_png)));

	recalcLayout();
}

ToolBox::ItemView::~ItemView()
{
	clearItems();
}

//===========================================================================
ToolBox::ControlWindow* ToolBox::ItemView::getWindow(void)
{
	return _Window;
}

cx::gw::Point ToolBox::ItemView::getItemViewSize(void)
{
	return _ItemViewSize;
}

//===========================================================================
bool ToolBox::ItemView::createDeviceResources(cx::gw::Context* ctx)
{
	//-----------------------------------------------------------------------
	bool rv;


	//-----------------------------------------------------------------------
	rv = _ItemViewDrawing->createDeviceResources(ctx);
	if (!rv)
	{
		return false;
	}

	for (auto& itemDrawing : _ItemDrawings)
	{
		rv = itemDrawing->createDeviceResources(ctx);
		if (!rv)
		{
			return false;
		}
	}

	rv = _BitmapList.createDeviceResources(ctx);
	if (!rv)
	{
		return false;
	}

	return true;
}

void ToolBox::ItemView::destroyDeviceResources(void)
{
	_BitmapList.destroyDeviceResources();


	for (auto& itemDrawing : _ItemDrawings)
	{
		itemDrawing->destroyDeviceResources();
	}

	_ItemViewDrawing->destroyDeviceResources();
}

void ToolBox::ItemView::draw(cx::gw::Context* ctx)
{
	_ItemViewDrawing->drawItemView(ctx, this);

	drawItems(ctx);
}

//===========================================================================
std::size_t ToolBox::ItemView::makeID(void)
{
	return ++_LastMadeID;
}

ToolBox::ItemSharedPtrs& ToolBox::ItemView::getItems(void)
{
	return _Items;
}

void ToolBox::ItemView::addItem(ToolBox::GroupItemSharedPtr parentItem, ToolBox::ItemSharedPtr item)
{
	if (parentItem)
	{
		item->setDepth(parentItem->getDepth() + 1);
		item->setParentItem(parentItem);

		parentItem->getSubItems().push_back(item);
	}
	else
	{
		item->setDepth(0);
		item->setParentItem(ToolBox::GroupItemSharedPtr());

		getItems().push_back(item);
	}

	item->getStatus()->setStatusChangedHandler(std::bind(&ToolBox::ItemView::onItemStatusChanged, this, item.get()));
}

void ToolBox::ItemView::removeItem(ToolBox::ItemSharedPtr item)
{
	if (!item)
	{
		return;
	}


	if (auto parentItem = item->getParentItem().lock())
	{
		removeItem(parentItem->getSubItems(), item);
	}
	else
	{
		removeItem(getItems(), item);
	}
}

void ToolBox::ItemView::removeItem(ToolBox::ItemSharedPtrs& items, ToolBox::ItemSharedPtr item)
{
	auto it = std::find(items.begin(), items.end(), item);

	if (it == items.end())
	{
		return;
	}

	item->getStatus()->setStatusChangedHandler(nullptr);

	items.erase(it);
}

void ToolBox::ItemView::clearItems(void)
{
	for (auto& item : getItems())
	{
		item->getStatus()->setStatusChangedHandler(nullptr);
	}

	getItems().clear();
}

void ToolBox::ItemView::clearItems(ToolBox::ItemSharedPtrs& items)
{
	for (auto& item : items)
	{
		item->getStatus()->setStatusChangedHandler(nullptr);
	}

	items.clear();
}

void ToolBox::ItemView::clearSubItems(ToolBox::GroupItemSharedPtr& groupItem)
{
	clearItems(groupItem->getSubItems());
}

//===========================================================================
ToolBox::ItemSharedPtr ToolBox::ItemView::findItem(std::size_t id)
{
	return findItem(getItems(), id);
}

ToolBox::ItemSharedPtr ToolBox::ItemView::findItem(ToolBox::ItemSharedPtrs& items, std::size_t id)
{
	for (auto& item : items)
	{
		if (item->getID() == id)
		{
			return item;
		}

		if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
		{
			if (!groupItem->getSubItems().empty())
			{
				auto subItem = findItem(groupItem->getSubItems(), id);
				if (subItem)
				{
					return subItem;
				}
			}
		}
	}

	return ToolBox::ItemSharedPtr();
}

//===========================================================================
void ToolBox::ItemView::recalcLayout(void)
{
	cx::gw::Point itemViewSize;
	itemViewSize = _ItemViewSize;


	std::int64_t window_cx;
	std::int64_t window_cy;
	getWindow()->getViewport()->getWindowSize(window_cx, window_cy);


	cx::gw::coord_t size;
	size = static_cast<cx::gw::coord_t>(window_cx);


	cx::gw::coord_t offset;
	offset = 0;
	for (auto& item : getItems())
	{
		offset = recalcItemLayout(offset, size, item);
	}


	_ItemViewSize = cx::gw::Point{ size, offset };
	if (_ItemViewSize._y != itemViewSize._y)
	{
		getWindow()->getViewport()->setDocumentSize(_ItemViewSize._x, _ItemViewSize._y);
	}
}

cx::gw::coord_t ToolBox::ItemView::recalcItemLayout(cx::gw::coord_t offset, cx::gw::coord_t size, ToolBox::ItemSharedPtr item)
{
	item->setX(0);
	item->setY(offset);

	item->setCX(size);
	item->setCY(item->getSize());

	offset += item->getSize();


	if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
	{
		if (!groupItem->getSubItems().empty())
		{
			if (!groupItem->isCollapseSubItems())
			{
				for (auto& subItem : groupItem->getSubItems())
				{
					offset = recalcItemLayout(offset, size, subItem);
				}
			}
		}
	}
	
	return offset;
}

//===========================================================================
cx::gw::BitmapList* ToolBox::ItemView::getBitmapList(void)
{
	return &_BitmapList;
}

//===========================================================================
ToolBox::ItemDrawingSharedPtr ToolBox::ItemView::getItemDrawing(ToolBox::ItemSharedPtr item)
{
	if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
	{
		return _GroupItemDrawing;
	}
	if (auto subItem = std::dynamic_pointer_cast<ToolBox::SubItem>(item))
	{
		return _SubItemDrawing;
	}

	return _GroupItemDrawing;
}

//===========================================================================
void ToolBox::ItemView::drawItems(cx::gw::Context* ctx)
{
	for (auto& item : getItems())
	{
		drawItem(ctx, item);
	}
}

void ToolBox::ItemView::drawItem(cx::gw::Context* ctx, ToolBox::ItemSharedPtr item)
{
	auto itemDrawing = getItemDrawing(item);
	if (itemDrawing)
	{
		itemDrawing->drawItem(ctx, this, item.get());

		if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
		{
			if (!groupItem->getSubItems().empty())
			{
				if (!groupItem->isCollapseSubItems())
				{
					for (auto& subItem : groupItem->getSubItems())
					{
						drawItem(ctx, subItem);
					}
				}
			}
		}
	}
}

//===========================================================================
void ToolBox::ItemView::onItemStatusChanged(ToolBox::Item* item)
{
	getWindow()->render();
}