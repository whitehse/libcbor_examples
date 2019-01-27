/*
 * Copyright (c) 2014-2019 Pavel Kalvoda <me@pavelkalvoda.com>
 *
 * libcbor is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "floats_ctrls.h"
#include "assert.h"
#include <math.h>

cbor_float_width cbor_float_get_width(const cbor_item_t *item)
{
	assert(cbor_isa_float_ctrl(item));
	return item->metadata.float_ctrl_metadata.width;
}

uint8_t cbor_ctrl_value(const cbor_item_t *item)
{
	assert(cbor_isa_float_ctrl(item));
	assert(cbor_float_get_width(item) == CBOR_FLOAT_0);
	return item->metadata.float_ctrl_metadata.ctrl;
}

bool cbor_float_ctrl_is_ctrl(const cbor_item_t *item)
{
	assert(cbor_isa_float_ctrl(item));
	return cbor_float_get_width(item) == CBOR_FLOAT_0;
}

float cbor_float_get_float2(const cbor_item_t *item)
{
	assert(cbor_is_float(item));
	assert(cbor_float_get_width(item) == CBOR_FLOAT_16);
	return *(float *) item->data;
}

float cbor_float_get_float4(const cbor_item_t *item)
{
	assert(cbor_is_float(item));
	assert(cbor_float_get_width(item) == CBOR_FLOAT_32);
	return *(float *) item->data;
}

double cbor_float_get_float8(const cbor_item_t *item)
{
	assert(cbor_is_float(item));
	assert(cbor_float_get_width(item) == CBOR_FLOAT_64);
	return *(double *) item->data;
}

double cbor_float_get_float(const cbor_item_t * item)
{
	assert(cbor_is_float(item));
	switch(cbor_float_get_width(item)) {
	case CBOR_FLOAT_0: return NAN;
	case CBOR_FLOAT_16: return cbor_float_get_float2(item);
	case CBOR_FLOAT_32: return cbor_float_get_float4(item);
	case CBOR_FLOAT_64: return cbor_float_get_float8(item);
	}
	return NAN; /* Compiler complaints */
}

void cbor_set_float2(cbor_item_t *item, float value)
{
	assert(cbor_is_float(item));
	assert(cbor_float_get_width(item) == CBOR_FLOAT_16);
	*((float *) item->data) = value;
}

void cbor_set_float4(cbor_item_t *item, float value)
{
	assert(cbor_is_float(item));
	assert(cbor_float_get_width(item) == CBOR_FLOAT_32);
	*((float *) item->data) = value;
}

void cbor_set_float8(cbor_item_t *item, double value)
{
	assert(cbor_is_float(item));
	assert(cbor_float_get_width(item) == CBOR_FLOAT_64);
	*((double *) item->data) = value;
}

void cbor_set_ctrl(cbor_item_t *item, uint8_t value)
{
	assert(cbor_isa_float_ctrl(item));
	assert(cbor_float_get_width(item) == CBOR_FLOAT_0);
	item->metadata.float_ctrl_metadata.ctrl = value;
}

bool cbor_ctrl_is_bool(const cbor_item_t *item)
{
	assert(cbor_is_bool(item));
	return item->metadata.float_ctrl_metadata.ctrl == CBOR_CTRL_TRUE;
}

cbor_item_t *cbor_new_ctrl()
{
	cbor_item_t *item = _CBOR_MALLOC(sizeof(cbor_item_t));
	*item = (cbor_item_t) {
		.type = CBOR_TYPE_FLOAT_CTRL,
		.data = NULL,
		.refcount = 1,
		.metadata = {.float_ctrl_metadata = {.width = CBOR_FLOAT_0, .ctrl = CBOR_CTRL_NONE}}
	};
	return item;
}

cbor_item_t *cbor_new_float2()
{
	cbor_item_t *item = _CBOR_MALLOC(sizeof(cbor_item_t) + 4);
	*item = (cbor_item_t) {
		.type = CBOR_TYPE_FLOAT_CTRL,
		.data = (unsigned char *) item + sizeof(cbor_item_t),
		.refcount = 1,
		.metadata = {.float_ctrl_metadata = {.width = CBOR_FLOAT_16}}
	};
	return item;
}

cbor_item_t *cbor_new_float4()
{
	cbor_item_t *item = _CBOR_MALLOC(sizeof(cbor_item_t) + 4);
	*item = (cbor_item_t) {
		.type = CBOR_TYPE_FLOAT_CTRL,
		.data = (unsigned char *) item + sizeof(cbor_item_t),
		.refcount = 1,
		.metadata = {.float_ctrl_metadata = {.width = CBOR_FLOAT_32}}
	};
	return item;
}

cbor_item_t *cbor_new_float8()
{
	cbor_item_t *item = _CBOR_MALLOC(sizeof(cbor_item_t) + 8);
	*item = (cbor_item_t) {
		.type = CBOR_TYPE_FLOAT_CTRL,
		.data = (unsigned char *) item + sizeof(cbor_item_t),
		.refcount = 1,
		.metadata = {.float_ctrl_metadata = {.width = CBOR_FLOAT_64}}
	};
	return item;
}

cbor_item_t *cbor_new_null()
{
	cbor_item_t *item = cbor_new_ctrl();
	cbor_set_ctrl(item, CBOR_CTRL_NULL);
	return item;
}

cbor_item_t *cbor_new_undef()
{
	cbor_item_t *item = cbor_new_ctrl();
	cbor_set_ctrl(item, CBOR_CTRL_UNDEF);
	return item;
}

cbor_item_t *cbor_build_bool(bool value)
{
	return cbor_build_ctrl(value ? CBOR_CTRL_TRUE : CBOR_CTRL_FALSE);
}

cbor_item_t *cbor_build_float2(float value)
{
	cbor_item_t *item = cbor_new_float2();
	cbor_set_float2(item, value);
	return item;
}

cbor_item_t *cbor_build_float4(float value)
{
	cbor_item_t *item = cbor_new_float4();
	cbor_set_float4(item, value);
	return item;
}

cbor_item_t *cbor_build_float8(double value)
{
	cbor_item_t *item = cbor_new_float8();
	cbor_set_float8(item, value);
	return item;
}

cbor_item_t *cbor_build_ctrl(uint8_t  value)
{
	cbor_item_t *item = cbor_new_ctrl();
	cbor_set_ctrl(item, value);
	return item;
}
