/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 SummerGift <SummerGift@qq.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"

#define MICROPY_PY_RECORDER 1
#if MICROPY_PY_RECORDER

typedef struct _recorder_obj_t {
    mp_obj_base_t base;
    const char* save_path;
} recorder_obj_t;

STATIC const mp_obj_type_t mp_recorder_type;
extern const mp_print_t mp_plat_print;

STATIC void error_check(bool status, const char *msg) {
    if (!status) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, msg));
    }
}

STATIC mp_obj_t recorder_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    recorder_obj_t *self = m_new_obj(recorder_obj_t);
    self->base.type = &mp_recorder_type;
    mp_arg_check_num(n_args, n_kw, 0, 0, true);
    return MP_OBJ_FROM_PTR(self);
}

//set a music to play
STATIC mp_obj_t recorder_record(mp_obj_t self_in, mp_obj_t path_obj) {
    const char* path = mp_obj_str_get_str(path_obj);
    recorder_obj_t *self = self_in;
    self->save_path = path;

    //Your code begin

    //Your code end

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(recorder_record_obj, recorder_record);

//stop playing, if this api is called, u should call opensong() to set a now song to play
STATIC mp_obj_t recorder_stop_song(mp_obj_t self_in) {
    recorder_obj_t *self = self_in;
    mp_int_t ret_val;

    //Your code begin

    //Your code end

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(recorder_stop_song_obj, recorder_stop_song);

STATIC const mp_rom_map_elem_t recorder_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_recorder) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_recorder), (mp_obj_t)&mp_recorder_type },	
	{ MP_ROM_QSTR(MP_QSTR_record), MP_ROM_PTR(&recorder_record_obj) },
	{ MP_ROM_QSTR(MP_QSTR_stop), MP_ROM_PTR(&recorder_stop_song_obj) },
};
STATIC MP_DEFINE_CONST_DICT(recorder_module_globals, recorder_module_globals_table);

STATIC const mp_obj_type_t mp_recorder_type= {
    { &mp_type_type },
    .name = MP_QSTR_recorder,
    .make_new = recorder_make_new,
    .locals_dict = (mp_obj_dict_t*)&recorder_module_globals,
};

const mp_obj_module_t mp_module_recorder = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&recorder_module_globals,
};

#endif // MICROPY_PY_PLAYER
